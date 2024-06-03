#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include <game/game.hpp>

#include "game_event.hpp"

#include <utils/concurrency.hpp>
#include <utils/hook.hpp>

namespace game_event
{
	namespace
	{
		using event_task = std::vector<std::function<void()>>;
		utils::concurrency::container<event_task> g_init_game_tasks;
		utils::concurrency::container<event_task> g_shutdown_game_tasks;

		void rope_init_ropes_stub()
		{
			g_init_game_tasks.access([](event_task& tasks)
			{
				for (const auto& func : tasks)
				{
					func();
				}
			});

			game::Rope_InitRopes();
		}

		void mantle_shutdown_anims_stub()
		{
			g_shutdown_game_tasks.access([](event_task& tasks)
			{
				for (const auto& func : tasks)
				{
					func();
				}
			});

			game::Mantle_ShutdownAnims();
		}
	}

	void on_g_init_game(const std::function<void()>& callback)
	{
		g_init_game_tasks.access([&callback](event_task& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	void on_g_shutdown_game(const std::function<void()>& callback)
	{
		g_shutdown_game_tasks.access([&callback](event_task& tasks)
		{
			tasks.emplace_back(callback);
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x1419DD6EC_g, rope_init_ropes_stub);
			utils::hook::call(0x141A02AAD_g, mantle_shutdown_anims_stub);
		}
	};
}

REGISTER_COMPONENT(game_event::component)
