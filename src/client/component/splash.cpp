#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "resource.hpp"

#include <utils/nt.hpp>
#include <utils/image.hpp>

namespace splash
{
	namespace
	{
		HWND window{};
		utils::image::object image{};
		std::thread window_thread{};

		utils::image::object load_splash_image()
		{
			const auto res = utils::nt::load_resource(IMAGE_SPLASH);
			const auto img = utils::image::load_image(res);
			return utils::image::create_bitmap(img);
		}

		void destroy_window()
		{
			if (window && IsWindow(window))
			{
				ShowWindow(window, SW_HIDE);
				DestroyWindow(window);
				window = nullptr;
			
				if (window_thread.joinable())
				{
					window_thread.join();
				}
			
				window = nullptr;
			}
			else if (window_thread.joinable())
			{
				window_thread.detach();
			}
		}

		void show()
		{
			const auto self = utils::nt::library::get_by_address(load_splash_image);
			const auto hInstance = utils::nt::library{};

			WNDCLASSA wndClass;
			wndClass.style = CS_DROPSHADOW;
			wndClass.cbClsExtra = NULL;
			wndClass.cbWndExtra = NULL;
			wndClass.lpszMenuName = nullptr;
			wndClass.lpfnWndProc = DefWindowProc;
			wndClass.hInstance = hInstance;
			wndClass.hIcon = LoadIcon(hInstance, RT_CURSOR);
			wndClass.hCursor = LoadCursor(nullptr, IDC_APPSTARTING);
			wndClass.hbrBackground = reinterpret_cast<HBRUSH>(6);
			wndClass.lpszClassName = "CoD Splash Screen";
			if (!RegisterClassA(&wndClass)) return;

			const auto screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
			const auto screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

			if (image == nullptr) return;

			window = CreateWindowEx(WS_EX_APPWINDOW, "CoD Splash Screen", "Black Ops III",
				WS_POPUP | WS_SYSMENU, (screenWidth - 320) / 2, (screenHeight - 100) / 2, 320,
				100, nullptr, nullptr, hInstance, nullptr);
			if (window == nullptr) return;

			const HWND codLogo = CreateWindowEx(NULL, "Static", nullptr, WS_VISIBLE | WS_CHILDWINDOW | SS_BITMAP, NULL, NULL, 320, 100, window, nullptr, self, nullptr);
			if (codLogo == nullptr) return;

			RECT rect;
			SendMessage(codLogo, STM_SETIMAGE, IMAGE_BITMAP, image);
			GetWindowRect(codLogo, &rect);

			const int width = rect.right - rect.left;
			const int height = rect.bottom - rect.top;
			rect.left = (screenWidth - width) / 2;
			rect.top = (screenHeight - height) / 2;
			rect.right = rect.left + width;
			rect.bottom = rect.top + height;

			AdjustWindowRect(&rect, WS_VISIBLE | WS_CHILDWINDOW | SS_BITMAP, NULL);
			SetWindowPos(window, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
			SetWindowRgn(window, CreateRoundRectRgn(NULL, NULL, rect.right - rect.left, rect.bottom - rect.top, 15, 15), TRUE);

			ShowWindow(window, SW_SHOW);
			UpdateWindow(window);
		}

		bool draw_frame()
		{
			if (!window) return false;

			MSG msg;
			bool success = true;
			while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_DESTROY && msg.hwnd == window) PostQuitMessage(0);
				if (msg.message == WM_QUIT) success = false;
			}
			return success;
		}

		void draw()
		{
			show();
			while (draw_frame())
			{
				std::this_thread::sleep_for(1ms);
			}

			window = nullptr;
			UnregisterClass("CoD Splash Screen", utils::nt::library{});
		}
	}

	struct component final : component_interface
	{
		component()
		{
			image = load_splash_image();
			window_thread = std::thread([this] {
				draw();
			});
		}

		void pre_destroy() override
		{
			destroy_window();
			if (window_thread.joinable())
			{
				window_thread.detach();
			}
		}

		void post_unpack() override
		{
			destroy_window();
		}
	};
}

REGISTER_COMPONENT(splash::component)
