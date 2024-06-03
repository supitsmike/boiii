#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include <game/game.hpp>

#include <utils/hook.hpp>

namespace status
{
	namespace
	{
		thread_local int g_client_num{0};

		void print_client_num(const int channel, const int label, const char* fmt, const int client_num)
		{
			g_client_num = client_num;
			game::Com_Printf(channel, label, fmt, client_num);
		}

		void print_client_xuid(const int channel, const int label, [[maybe_unused]] const char* fmt, const uint64_t xuid)
		{
			if (game::SV_IsTestClient(g_client_num))
			{
				game::Com_Printf(channel, label, "%16s ", "bot0");
				return;
			}

			game::Com_Printf(channel, label, "%12llx ", xuid);
		}
	}

	struct component final : generic_component
	{
		void post_unpack() override
		{
			// Patch the status command for test clients
			utils::hook::call(0x142246E37_g, print_client_num);
			utils::hook::call(0x142246EDE_g, print_client_xuid);

			utils::hook::copy_string(0x143050480_g,
			                         "num score ping xuid             name             address                  qport  \n");
			utils::hook::copy_string(0x1430504E0_g,
			                         "--- ----- ---- ---------------- ---------------- ------------------------ ------ \n");
		}
	};
}

REGISTER_COMPONENT(status::component)
