#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <utils/hook.hpp>

#include "game/game.hpp"

namespace demonware
{
	namespace
	{

	}

	struct component final : component_interface
	{
		void post_unpack() override
		{
			utils::hook::set<uint8_t>(0x141E0AA1B_g, 0xEB); // Release un-handled reportReward spamming loop
		}
	};
}

REGISTER_COMPONENT(demonware::component)
