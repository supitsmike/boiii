#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

#include <game/game.hpp>
#include "getinfo.hpp"

namespace bots
{
	namespace
	{

	}

	struct component final : component_interface
	{
		static_assert(offsetof(game::client_s, bIsTestClient) == 0xBB360);

		void post_unpack() override
		{
			command::add("spawnBot", [](const command::params& params)
			{
				if (!getinfo::is_host()) return;

				size_t count = 1;
				if (params.size() > 1)
				{
					if (params[1] == "all"s) count = 18;
					else count = atoi(params[1]);
				}

				scheduler::once([count]
				{
					for (size_t i = 0; i < count; ++i)
					{
						if (!game::SV_AddTestClient()) break;
					}
				}, scheduler::server);
			});
		}
	};
}

REGISTER_COMPONENT(bots::component)
