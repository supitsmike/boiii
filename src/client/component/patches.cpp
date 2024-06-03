#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <game/game.hpp>
#include <game/utils.hpp>

#include <utils/hook.hpp>

namespace patches
{
	namespace
	{
		const game::dvar_t* lobby_min_players;

		void script_errors_stub([[maybe_unused]] const char* file, [[maybe_unused]] int line,
		                        [[maybe_unused]] unsigned int code, const char* fmt, ...)
		{
			char buffer[0x1000];

			{
				va_list ap;
				va_start(ap, fmt);
				vsnprintf_s(buffer, _TRUNCATE, fmt, ap);
				va_end(ap);
			}

			game::Com_Error(game::ERROR_SCRIPT_DROP, "%s", buffer);
		}

		void scr_get_num_expected_players()
		{
			auto expected_players = game::LobbyHost_GetClientCount(game::LOBBY_TYPE_GAME,
			                                                       game::LOBBY_CLIENT_TYPE_ALL);

			const auto mode = game::Com_SessionMode_GetMode();
			if ((mode == game::MODE_ZOMBIES || mode == game::MODE_CAMPAIGN))
			{
				if (const auto min_players = lobby_min_players->current.value.integer)
				{
					expected_players = min_players;
				}
			}

			const auto num_expected_players = std::max(1, expected_players);
			game::Scr_AddInt(game::SCRIPTINSTANCE_SERVER, num_expected_players);
		}

		void sv_execute_client_messages_stub(game::client_s* client, game::msg_t* msg)
		{
			if ((client->reliableSequence - client->reliableAcknowledge) < 0)
			{
				client->reliableAcknowledge = client->reliableSequence;
				game::SV_DropClient(client, "EXE_LOSTRELIABLECOMMANDS", true, true);
				return;
			}

			game::SV_ExecuteClientMessage(client, msg);
		}
	}

	struct component final : component_interface
	{
		void post_unpack() override
		{
			// print hexadecimal xuids in chat game log command
			utils::hook::set<char>(0x142FD9362_g, 'x');

			// don't make script errors fatal error
			utils::hook::call(0x1412CAC4D_g, script_errors_stub);

			// change 4 character name limit to 3 characters
			utils::hook::set<uint8_t>(0x14224DA53_g, 3);
			utils::hook::set<uint8_t>(0x14224DBB4_g, 3);
			utils::hook::set<uint8_t>(0x14224DF8C_g, 3);

			// make sure reliableAck is not negative or too big
			utils::hook::call(0x14225489C_g, sv_execute_client_messages_stub);

			lobby_min_players = game::register_dvar_int("lobby_min_players", 0, 0, 8, game::DVAR_NONE, "");
			utils::hook::jump(0x141A7BCF0_g, scr_get_num_expected_players, true);
		}
	};
}

REGISTER_COMPONENT(patches::component)
