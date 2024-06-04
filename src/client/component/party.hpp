#pragma once
#include <utils/info_string.hpp>

#include "game/game.hpp"

namespace party
{
	bool is_host(const game::netadr_t& addr);
}
