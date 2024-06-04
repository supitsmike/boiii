#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include "party.hpp"
#include "network.hpp"
#include "scheduler.hpp"
#include "workshop.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/info_string.hpp>
#include <utils/cryptography.hpp>
#include <utils/concurrency.hpp>

namespace party
{
	namespace
	{

	}

	game::netadr_t get_connected_server()
	{
		constexpr auto local_client_num = 0ull;
		const auto address = *reinterpret_cast<uint64_t*>(0x1453D8BB8_g) + (0x25780 * local_client_num) + 0x10;
		return *reinterpret_cast<game::netadr_t*>(address);
	}

	bool is_host(const game::netadr_t& addr)
	{
		return get_connected_server() == addr;
	}

	struct component final : component_interface
	{

	};
}

REGISTER_COMPONENT(party::component)
