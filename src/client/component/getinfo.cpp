#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "network.hpp"
#include "workshop.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/info_string.hpp>

#include "game/utils.hpp"

namespace getinfo
{
	int get_max_client_count()
	{
		return game::get_dvar_int("com_maxclients");
	}

	template <typename T>
	int get_client_count(T* client_states)
	{
		if (!client_states)
		{
			return 0;
		}

		int count = 0;
		for (int i = 0; i < get_max_client_count(); ++i)
		{
			if (client_states[i].client_state > 0)
			{
				++count;
			}
		}

		return count;
	}

	size_t get_client_count()
	{
		size_t count = 0;
		game::foreach_connected_client([&count](const game::client_s&)
		{
			++count;
		});

		return count;
	}

	size_t get_bot_count()
	{
		size_t count = 0;

		game::foreach_connected_client([&count](const game::client_s&, const size_t index)
		{
			if (game::SV_IsTestClient(static_cast<int>(index)))
			{
				++count;
			}
		});

		return count;
	}

	int get_assigned_team()
	{
		return (rand() % 2) + 1;
	}

	bool is_host()
	{
		return game::SV_Loaded() && !game::Com_IsRunningUILevel();
	}

	struct component final : component_interface
	{
		void post_unpack() override
		{
			//utils::hook::jump(game::select(0x142254EF0, 0x140537730), get_assigned_team);
		}
	};
}

// REGISTER_COMPONENT(getinfo::component)
