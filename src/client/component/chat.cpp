#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "chat.hpp"

#include "game/game.hpp"
#include "game/utils.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#include "command.hpp"

namespace chat
{
	namespace
	{
		const game::dvar_t* g_deadChat;

		void cmd_say_f(game::gentity_s* ent, const command::params_sv& params)
		{
			if (params.size() < 2)
			{
				return;
			}

			int mode = 0;
			if (params[0] == "say_team"s)
			{
				mode = 1;
			}

			const auto p = params.join(1);
			game::Scr_AddString(game::SCRIPTINSTANCE_SERVER, p.data() + 1); // Skip special char
			game::Scr_Notify_Canon(ent, game::Scr_CanonHash(params[0]), 1);

			game::G_Say(ent, nullptr, mode, p.data());
		}

		void cmd_chat_f(game::gentity_s* ent, const command::params_sv& params)
		{
			auto p = params.join(1);

			// Not a mistake! + 2 is necessary for the GSC script to receive only the actual chat text
			game::Scr_AddString(game::SCRIPTINSTANCE_SERVER, p.data() + 2);
			game::Scr_Notify_Canon(ent, game::Scr_CanonHash(params[0]), 1);

			utils::hook::invoke<void>(0x140298E70_g, ent, p.data());
		}

		uint64_t* divert_xuid_to_client_num_stub(int, const int client_num, int)
		{
			static thread_local uint64_t value;
			// zero xuid is invalid, so increase the clientnum to prevent 0 values
			value = static_cast<uint64_t>(client_num) + 1;
			return &value;
		}

		void send_chat_message(int client_num, const std::string& text)
		{
			game::SV_GameSendServerCommand(client_num, game::SV_CMD_RELIABLE_0, utils::string::va("v \"%Iu %d %d %s\"", -1, 0, 0, text.data()));
		}

		// This function has probably a different name
		void g_say_to_stub(utils::hook::assembler& a)
		{
			const auto no_dead_chat = a.newLabel();

			// game's code
			a.mov(rax, qword_ptr(rbx));

			a.push(rax);

			a.mov(rax, qword_ptr(reinterpret_cast<std::uintptr_t>(&g_deadChat)));
			a.mov(al, byte_ptr(rax, 0x28)); // dvar_t.current.value.enabled
			a.test(al, al);

			a.pop(rax);

			a.je(no_dead_chat);

			a.jmp(0x140299061_g);

			a.bind(no_dead_chat);
			a.cmp(dword_ptr(rax, 0x16AE0), 0x0); // game's code
			a.jmp(0x14029905B_g);
		}

		void cl_handle_chat(char* dest, size_t dest_size, const char* src)
		{
			game::I_strcpy(dest, dest_size, src);
			printf("%s\n", dest);
		}
	}

	const char* get_client_name(const uint64_t xuid)
	{
		if (xuid < 19)
		{
			char buffer[256]{};
			game::CL_GetClientName(0, static_cast<int>(xuid - 1), buffer, sizeof(buffer), true);

			return utils::string::va("%s\n", buffer);
		}

		return "Unknown Soldier";
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x141974B04_g, divert_xuid_to_client_num_stub);

			// Ignore some check that suppresses the chat
			utils::hook::nop(0x141DEA9BD_g, 2);

			// Add chat history to the in-game console
			utils::hook::call(0x141DEAA0F_g, cl_handle_chat); // I_strcpy
		}
	};
}

REGISTER_COMPONENT(chat::component)
