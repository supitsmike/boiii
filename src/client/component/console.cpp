#include <std_include.hpp>
#include "console.hpp"
#include "loader/component_loader.hpp"
#include "resource.hpp"

#include "game/game.hpp"

#include <utils/thread.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>
#include <utils/image.hpp>

constexpr auto console_buffer_size = 0x4000;
constexpr auto window_width = 608;

namespace console
{
	namespace
	{
		utils::image::object codLogo;
		std::atomic_bool started{false};
		std::atomic_bool terminate_runner{false};
		utils::concurrency::container<std::function<void(const std::string& message)>> interceptor{};
		utils::concurrency::container<std::queue<std::string>> message_queue{};

		utils::hook::detour Sys_CreateConsole_Stub;

		void queue_message(const char* message)
		{
			interceptor.access([message](const std::function<void(const std::string&)>& callback)
			{
				if (callback)
				{
					callback(message);
				}
			});

			message_queue.access([message](std::queue<std::string>& queue)
			{
				queue.emplace(message);
			});
		}

		std::queue<std::string> empty_message_queue()
		{
			std::queue<std::string> current_queue{};

			message_queue.access([&](std::queue<std::string>& queue)
			{
				current_queue = std::move(queue);
				queue = {};
			});

			return current_queue;
		}

		void printf_Hook(const char* fmt, ...)
		{
			va_list ap;
			va_start(ap, fmt);

			char buffer[1024]{};
			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, ap);

			if (started && !terminate_runner)
			{
				game::Com_Printf(0, 0, "%s", buffer);
			}

			va_end(ap);
		}

		LRESULT ConWndProc(HWND hWnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORSTATIC:
			{
				const auto hdc = reinterpret_cast<HDC>(wParam);
				SetBkColor(hdc, RGB(36, 36, 36));
				SetTextColor(hdc, RGB(240, 240, 240));

				static utils::image::object b(CreateSolidBrush(RGB(36, 36, 36)));
				return reinterpret_cast<INT_PTR>(b.get());
			}
			case WM_CLOSE:
				game::Cbuf_AddText(0, "quit\n");
				[[fallthrough]];
			default:
				return utils::hook::invoke<LRESULT>(0x142332960_g, hWnd, uMsg, wParam, lParam);
			}
		}

		void Sys_CreateConsole_Hook(const HINSTANCE hInstance)
		{
			char text[console_buffer_size]{0};

			WNDCLASSA wnd_class{};
			wnd_class.style = NULL;
			wnd_class.lpfnWndProc = ConWndProc;
			wnd_class.cbClsExtra = NULL;
			wnd_class.cbWndExtra = NULL;
			wnd_class.hInstance = hInstance;
			wnd_class.hIcon = LoadIcon(hInstance, RT_CURSOR);
			wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wnd_class.hbrBackground = CreateSolidBrush(RGB(50, 50, 50));
			wnd_class.lpszMenuName = nullptr;
			wnd_class.lpszClassName = "CoD Black Ops III WinConsole";
			if (!RegisterClass(&wnd_class)) return;

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = 620;
			rect.bottom = 450;
			AdjustWindowRect(&rect, 0x80CA0000, 0);

			auto hdc = GetDC(GetDesktopWindow());
			const auto windowWidth = GetDeviceCaps(hdc, HORZRES);
			const auto windowHeight = GetDeviceCaps(hdc, VERTRES);
			ReleaseDC(GetDesktopWindow(), hdc);

			utils::hook::set<int>(game::s_wcd::windowWidth, (rect.right - rect.left + 1));
			utils::hook::set<int>(game::s_wcd::windowHeight, (rect.bottom - rect.top + 1));

			utils::hook::set<HWND>(game::s_wcd::hWnd, CreateWindowEx(
				                       NULL, "CoD Black Ops III WinConsole", "CoD Black Ops III Console", WS_POPUP | 0xCA0000,
				                       (windowWidth - 600) / 2, (windowHeight - 450) / 2, rect.right - rect.left + 1,
				                       rect.bottom - rect.top + 1, nullptr, nullptr, hInstance, nullptr));
			if (!*game::s_wcd::hWnd) return;

			hdc = GetDC(*game::s_wcd::hWnd);
			const auto cHeight = MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
			utils::hook::set<HFONT>(game::s_wcd::hfBufferFont,
			                        CreateFont(-cHeight, 0, 0, 0, 300, NULL, NULL, NULL, DEFAULT_CHARSET, NULL, NULL,
			                                   NULL, FIXED_PITCH | FF_MODERN, "Courier New"));
			ReleaseDC(*game::s_wcd::hWnd, hdc);

			if (codLogo)
			{
				utils::hook::set<HWND>(game::s_wcd::codLogo,
				                       CreateWindowEx(0, "Static", nullptr, WS_VISIBLE | WS_CHILDWINDOW | SS_BITMAP, 5,
				                                      5, 0, 0, *game::s_wcd::hWnd, reinterpret_cast<HMENU>(1),
				                                      hInstance, nullptr));
				SendMessage(*game::s_wcd::codLogo, STM_SETIMAGE, IMAGE_BITMAP, codLogo);
			}

			utils::hook::set<HWND>(game::s_wcd::hwndInputLine,
			                       CreateWindowEx(0, "edit", nullptr,
			                                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 6, 400,
			                                      window_width, 20, *game::s_wcd::hWnd, reinterpret_cast<HMENU>(101),
			                                      hInstance, nullptr));
			utils::hook::set<HWND>(game::s_wcd::hwndBuffer,
			                       CreateWindowEx(0, "edit", nullptr,
			                                      WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL |
			                                      ES_MULTILINE, 6, 70, window_width, 324, *game::s_wcd::hWnd,
			                                      reinterpret_cast<HMENU>(100), hInstance, nullptr));
			SendMessage(*game::s_wcd::hwndBuffer, WM_SETFONT, reinterpret_cast<WPARAM>(*game::s_wcd::hfBufferFont), NULL);

			utils::hook::set<WNDPROC>(game::s_wcd::SysInputLineWndProc,
			                          reinterpret_cast<WNDPROC>(SetWindowLongPtr(
				                          *game::s_wcd::hwndInputLine, GWLP_WNDPROC,
										  reinterpret_cast<LONG_PTR>(*game::InputLineWndProc))));
			SendMessage(*game::s_wcd::hwndInputLine, WM_SETFONT, reinterpret_cast<WPARAM>(*game::s_wcd::hfBufferFont), 0);

			SetFocus(*game::s_wcd::hwndInputLine);
			game::Con_GetTextCopy(text, std::min(console_buffer_size, static_cast<int>(sizeof(text))));
			SetWindowText(*game::s_wcd::hwndBuffer, text);
		}
	}

	void set_interceptor(std::function<void(const std::string& message)> callback)
	{
		interceptor.access([&callback](std::function<void(const std::string&)>& c)
		{
			c = std::move(callback);
		});
	}

	void remove_interceptor()
	{
		set_interceptor({});
	}

	void set_title(const std::string& title)
	{
		SetWindowTextA(*game::s_wcd::hWnd, title.data());
	}

	struct component final : component_interface
	{
		void post_unpack() override
		{
			utils::hook::set<uint8_t>(0x14133D2FE_g, 0xEB); // Always enable ingame console
			utils::hook::jump(0x141344E44_g, 0x141344E2E_g); // Remove the need to type '\' or '/' to send a console command

			utils::hook::jump(printf, printf_Hook);

			utils::hook::jump(0x142332C30_g, queue_message);
			utils::hook::nop(0x142332C4A_g, 2); // Print from every thread

			const auto res = utils::nt::load_resource(IMAGE_LOGO);
			const auto img = utils::image::load_image(res);
			codLogo = utils::image::create_bitmap(img);

			terminate_runner = false;

			this->message_runner_ = utils::thread::create_named_thread("Console IO", []
			{
				while (!terminate_runner)
				{
					std::string message_buffer{};
					auto current_queue = empty_message_queue();

					while (!current_queue.empty())
					{
						const auto& msg = current_queue.front();
						message_buffer.append(msg);
						current_queue.pop();
					}

					if (!message_buffer.empty())
					{
						static auto print_func = utils::hook::assemble([](utils::hook::assembler& a) {
							a.push(rbx);
							a.mov(eax, 0x8030);
							a.jmp(0x142332AA7_g);
						});

						static_cast<void(*)(const char*)>(print_func)(message_buffer.data());
					}

					std::this_thread::sleep_for(5ms);
				}
			});

			this->console_runner_ = utils::thread::create_named_thread("Console Window", [this]
			{
				{
					Sys_CreateConsole_Stub.create(0x142332E00_g, Sys_CreateConsole_Hook);

					game::Sys_ShowConsole();
					started = true;
				}

				MSG msg{};
				while (!terminate_runner)
				{
					if (PeekMessageW(&msg, nullptr, NULL, NULL, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessageW(&msg);
					}
					else
					{
						std::this_thread::sleep_for(5ms);
					}
				}
			});

			while (!started)
			{
				std::this_thread::sleep_for(10ms);
			}
		}

		void pre_destroy() override
		{
			terminate_runner = true;

			if (this->message_runner_.joinable())
			{
				this->message_runner_.join();
			}

			if (this->console_runner_.joinable())
			{
				this->console_runner_.join();
			}
		}

	private:
		std::thread console_runner_{};
		std::thread message_runner_{};
	};
}

REGISTER_COMPONENT(console::component)
