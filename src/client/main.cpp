#include <std_include.hpp>

#include "loader/component_loader.hpp"
#include "loader/loader.hpp"

#include <utils/finally.hpp>
#include <utils/hook.hpp>
#include <utils/nt.hpp>
#include <utils/io.hpp>
#include <utils/flags.hpp>

#include <steam/steam.hpp>

#include "game/game.hpp"

namespace
{
	volatile bool g_call_tls_callbacks = false;
	std::pair<void**, void*> g_original_import{};

	DECLSPEC_NORETURN void WINAPI exit_hook(const uint32_t code)
	{
		component_loader::pre_destroy();
		ExitProcess(code);
	}

	std::pair<void**, void*> patch_steam_import(const std::string& func, void* function)
	{
		static const utils::nt::library game{};

		const auto game_entry = game.get_iat_entry("steam_api64.dll", func);
		if (!game_entry)
		{
			//throw std::runtime_error("Import '" + func + "' not found!");
			return {nullptr, nullptr};
		}

		const auto original_import = game_entry;
		utils::hook::set(game_entry, function);
		return {game_entry, original_import};
	}

	bool restart_app_if_necessary_stub()
	{
		const std::string steam_path = steam::SteamAPI_GetSteamInstallPath();
		if (steam_path.empty() || !::utils::io::file_exists(steam_path + "/steam.exe"))
		{
			game::show_error("Steam must be installed for the game to run. Please install Steam!");
			ShellExecuteA(nullptr, "open", "https://store.steampowered.com/about/", nullptr, nullptr, SW_SHOWNORMAL);
			TerminateProcess(GetCurrentProcess(), 1);
		}

		utils::hook::set(g_original_import.first, g_original_import.second);
		patch_steam_import("SteamAPI_Shutdown", steam::SteamAPI_Shutdown);

		component_loader::post_unpack();
		return steam::SteamAPI_RestartAppIfNecessary();
	}

	BOOL set_process_dpi_aware_stub()
	{
		component_loader::post_unpack();
		return SetProcessDPIAware();
	}

	void patch_imports()
	{
		patch_steam_import("SteamAPI_RegisterCallback", steam::SteamAPI_RegisterCallback);
		patch_steam_import("SteamAPI_RegisterCallResult", steam::SteamAPI_RegisterCallResult);
		patch_steam_import("SteamGameServer_Shutdown", steam::SteamGameServer_Shutdown);
		patch_steam_import("SteamGameServer_RunCallbacks", steam::SteamGameServer_RunCallbacks);
		patch_steam_import("SteamGameServer_GetHSteamPipe", steam::SteamGameServer_GetHSteamPipe);
		patch_steam_import("SteamGameServer_GetHSteamUser", steam::SteamGameServer_GetHSteamUser);
		patch_steam_import("SteamInternal_GameServer_Init", steam::SteamInternal_GameServer_Init);
		patch_steam_import("SteamAPI_UnregisterCallResult", steam::SteamAPI_UnregisterCallResult);
		patch_steam_import("SteamAPI_UnregisterCallback", steam::SteamAPI_UnregisterCallback);
		patch_steam_import("SteamAPI_RunCallbacks", steam::SteamAPI_RunCallbacks);
		patch_steam_import("SteamInternal_CreateInterface", steam::SteamInternal_CreateInterface);
		patch_steam_import("SteamAPI_GetHSteamUser", steam::SteamAPI_GetHSteamUser);
		patch_steam_import("SteamAPI_GetHSteamPipe", steam::SteamAPI_GetHSteamPipe);
		patch_steam_import("SteamAPI_Init", steam::SteamAPI_Init);
		//patch_steam_import("SteamAPI_Shutdown", steam::SteamAPI_Shutdown);
		g_original_import = patch_steam_import("SteamAPI_RestartAppIfNecessary", restart_app_if_necessary_stub);

		const utils::nt::library game{};
		utils::hook::set(game.get_iat_entry("kernel32.dll", "ExitProcess"), exit_hook);
		utils::hook::set(game.get_iat_entry("user32.dll", "SetProcessDPIAware"), set_process_dpi_aware_stub);
	}

	void remove_crash_file()
	{
		const utils::nt::library game{};
		const auto game_file = game.get_path();
		auto game_path = std::filesystem::path(game_file);
		game_path.replace_extension(".start");

		utils::io::remove_file(game_path);
	}

	PIMAGE_TLS_CALLBACK* get_tls_callbacks()
	{
		const utils::nt::library game{};
		const auto& entry = game.get_optional_header()->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
		if (!entry.VirtualAddress || !entry.Size)
		{
			return nullptr;
		}

		const auto* tls_dir = reinterpret_cast<IMAGE_TLS_DIRECTORY*>(game.get_ptr() + entry.VirtualAddress);
		return reinterpret_cast<PIMAGE_TLS_CALLBACK*>(tls_dir->AddressOfCallBacks);
	}

	void run_tls_callbacks(const DWORD reason)
	{
		if (!g_call_tls_callbacks)
		{
			return;
		}

		auto* callback = get_tls_callbacks();
		while (callback && *callback)
		{
			(*callback)(GetModuleHandleA(nullptr), reason, nullptr);
			++callback;
		}
	}

	[[maybe_unused]] thread_local struct tls_runner
	{
		tls_runner()
		{
			run_tls_callbacks(DLL_THREAD_ATTACH);
		}

		~tls_runner()
		{
			run_tls_callbacks(DLL_THREAD_DETACH);
		}
	} tls_runner;

	FARPROC load_process(const std::string& procname)
	{
		const auto proc = loader::load_binary(procname);

		auto* const peb = reinterpret_cast<PPEB>(__readgsqword(0x60));
		peb->Reserved3[1] = proc.get_ptr();
		static_assert(offsetof(PEB, Reserved3[1]) == 0x10);

		return FARPROC(proc.get_ptr() + proc.get_relative_entry_point());
	}

	bool handle_process_runner()
	{
		const auto* const command = "-proc ";
		const char* parent_proc = strstr(GetCommandLineA(), command);

		if (!parent_proc)
		{
			return false;
		}

		const auto pid = DWORD(atoi(parent_proc + strlen(command)));
		const utils::nt::handle<> process_handle = OpenProcess(SYNCHRONIZE, FALSE, pid);
		if (process_handle)
		{
			WaitForSingleObject(process_handle, INFINITE);
		}

		return true;
	}

	void enable_dpi_awareness()
	{
		const utils::nt::library user32{"user32.dll"};

		{
			const auto set_dpi = user32
				                     ? user32.get_proc<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>(
					                     "SetProcessDpiAwarenessContext")
				                     : nullptr;
			if (set_dpi)
			{
				set_dpi(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
				return;
			}
		}

		{
			const utils::nt::library shcore{"shcore.dll"};
			const auto set_dpi = shcore
				                     ? shcore.get_proc<HRESULT(WINAPI*)(PROCESS_DPI_AWARENESS)>(
					                     "SetProcessDpiAwareness")
				                     : nullptr;
			if (set_dpi)
			{
				set_dpi(PROCESS_PER_MONITOR_DPI_AWARE);
				return;
			}
		}

		{
			const auto set_dpi = user32
				                     ? user32.get_proc<BOOL(WINAPI*)()>(
					                     "SetProcessDPIAware")
				                     : nullptr;
			if (set_dpi)
			{
				set_dpi();
			}
		}
	}

	void trigger_high_performance_gpu_switch()
	{
		const auto key = utils::nt::open_or_create_registry_key(
			HKEY_CURRENT_USER, R"(Software\Microsoft\DirectX\UserGpuPreferences)");
		if (!key)
		{
			return;
		}

		const auto self = utils::nt::library::get_by_address(&trigger_high_performance_gpu_switch);
		const auto path = self.get_path().make_preferred().wstring();

		if (RegQueryValueExW(key, path.data(), nullptr, nullptr, nullptr, nullptr) != ERROR_FILE_NOT_FOUND)
		{
			return;
		}

		const std::wstring data = L"GpuPreference=2;";
		RegSetValueExW(key, self.get_path().make_preferred().wstring().data(), 0, REG_SZ,
		               reinterpret_cast<const BYTE*>(data.data()),
		               static_cast<DWORD>((data.size() + 1u) * 2));
	}

	void validate_non_network_share()
	{
		const auto self = utils::nt::library::get_by_address(&validate_non_network_share);
		const auto path = self.get_path().make_preferred();
		const auto wpath = path.wstring();
		if (wpath.size() >= 2 && wpath[0] == L'\\' && wpath[1] == L'\\')
		{
			throw std::runtime_error(
				"You seem to be using a network share:\n\n" + path.string() + "\n\nNetwork shares are not supported!");
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		srand(uint32_t(time(nullptr)) ^ ~(GetTickCount() * GetCurrentProcessId()));

		enable_dpi_awareness();

		{
			auto premature_shutdown = true;
			const auto _ = utils::finally([&premature_shutdown]
				{
					if (premature_shutdown)
					{
						component_loader::pre_destroy();
					}
				});

			try
			{
				validate_non_network_share();
				remove_crash_file();

				trigger_high_performance_gpu_switch();

				if (!component_loader::activate(false))
				{
					return 1;
				}

				patch_imports();

				if (!component_loader::post_load())
				{
					return 1;
				}

				premature_shutdown = false;
			}
			catch (std::exception& e)
			{
				game::show_error(e.what());
				return 1;
			}
		}

		g_call_tls_callbacks = true;
	}

	return TRUE;
}

extern "C" __declspec(dllexport)
HRESULT D3D11CreateDevice(void* adapter, const uint64_t driver_type,
	const HMODULE software, const UINT flags,
	const void* p_feature_levels, const UINT feature_levels,
	const UINT sdk_version, void** device, void* feature_level,
	void** immediate_context)
{
	static auto func = []
		{
			char dir[MAX_PATH]{ 0 };
			GetSystemDirectoryA(dir, sizeof(dir));

			const auto d3d11 = utils::nt::library::load(dir + "/d3d11.dll"s);
			return d3d11.get_proc<decltype(&D3D11CreateDevice)>("D3D11CreateDevice");
		}();

	return func(adapter, driver_type, software, flags, p_feature_levels, feature_levels, sdk_version, device,
		feature_level, immediate_context);
}
