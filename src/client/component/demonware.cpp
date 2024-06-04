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
			utils::hook::set<uint32_t>(0x141EC4AC0_g, 0xC3D08948); // Skip publisher file signature stuff
			utils::hook::set<uint64_t>(0x141F04500_g, 0xC300000001B8); // Kill LPC_File_SafeWrite
			utils::hook::set<uint64_t>(0x141F03130_g, 0xC300000001B8); // Kill LPC_DeleteStale

			utils::hook::set<uint8_t>(0x141E0AA1B_g, 0xEB); // Release un-handled reportReward spamming loop
		}
	};
}

REGISTER_COMPONENT(demonware::component)
