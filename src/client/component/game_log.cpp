#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/utils.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace game_log
{
	namespace
	{
		void g_scr_log_print()
		{
			char string[1024]{};
			std::size_t i_string_len = 0;

			const auto i_num_parms = game::Scr_GetNumParam(game::SCRIPTINSTANCE_SERVER);
			for (std::uint32_t i = 0; i < i_num_parms; ++i)
			{
				const auto* psz_token = game::Scr_GetString(game::SCRIPTINSTANCE_SERVER, i);
				const auto i_token_len = std::strlen(psz_token);

				i_string_len += i_token_len;
				if (i_string_len >= sizeof(string))
				{
					// Do not overflow the buffer
					break;
				}

				strncat_s(string, psz_token, _TRUNCATE);
			}

			game::G_LogPrintf("%s", string);
		}

		void g_log_printf_stub(const char* fmt, ...)
		{
			char va_buffer[0x400] = { 0 };

			va_list ap;
			va_start(ap, fmt);
			vsnprintf_s(va_buffer, _TRUNCATE, fmt, ap);
			va_end(ap);

			const auto* file = "games_mp.log";
			const auto time = *game::level_time / 1000;

			utils::io::write_file(file, utils::string::va("%3i:%i%i %s",
				time / 60,
				time % 60 / 10,
				time % 60 % 10,
				va_buffer
			), true);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Fix format string vulnerability & make it work
			utils::hook::jump(0x141AAFA00_g, g_scr_log_print);
			utils::hook::jump(0x1419BFA50_g, g_log_printf_stub);
		}
	};
}

//REGISTER_COMPONENT(game_log::component)
