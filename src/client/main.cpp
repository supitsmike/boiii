#include <std_include.hpp>

#include "loader/component_loader.hpp"
#include "loader/loader.hpp"

#include <utils/finally.hpp>
#include <utils/hook.hpp>
#include <utils/nt.hpp>
#include <utils/io.hpp>
#include <utils/flags.hpp>

#include "game/game.hpp"
#include "proxy/proxy.hpp"

namespace
{
	volatile bool g_call_tls_callbacks = false;

	DECLSPEC_NORETURN void WINAPI exit_hook(const uint32_t code)
	{
		component_loader::pre_destroy();
		ExitProcess(code);
	}

	BOOL set_process_dpi_aware_stub()
	{
		component_loader::post_unpack();
		return SetProcessDPIAware();
	}

	void patch_imports()
	{
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

	void enable_dpi_awareness()
	{
		const utils::nt::library user32{ "user32.dll" };

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
			const utils::nt::library shcore{ "shcore.dll" };
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
		// Make sure to link D3D11, as this might trigger high performance GPU
		static volatile auto _ = &D3D11CreateDevice;

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
		srand(static_cast<uint32_t>(time(nullptr)) ^ ~(GetTickCount() * GetCurrentProcessId()));

		proxy::setup_proxy();
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

				if (!component_loader::activate())
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
