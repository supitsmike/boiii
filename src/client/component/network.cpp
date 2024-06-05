#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "scheduler.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/finally.hpp>

#include "network.hpp"

#include "game/fragment_handler.hpp"

namespace network
{
	namespace
	{
		std::unordered_map<std::string, callback>& get_callbacks()
		{
			static std::unordered_map<std::string, callback> callbacks{};
			return callbacks;
		}
	}

	void on(const std::string& command, const callback& callback)
	{
		get_callbacks()[utils::string::to_lower(command)] = callback;
	}

	void send(const game::netadr_t& address, const std::string& command, const std::string& data, const char separator)
	{
		std::string packet = "\xFF\xFF\xFF\xFF";
		packet.append(command);
		packet.push_back(separator);
		packet.append(data);

		send_data(address, packet);
	}

	sockaddr_in convert_to_sockaddr(const game::netadr_t& address)
	{
		sockaddr_in to{};
		to.sin_family = AF_INET;
		to.sin_port = htons(address.port);
		to.sin_addr.S_un.S_addr = address.addr;
		return to;
	}

	void send_data(const game::netadr_t& address, const void* data, const size_t length)
	{
		//game::NET_SendPacket(game::NS_CLIENT1, static_cast<int>(size), data, &address);

		const auto to = convert_to_sockaddr(address);
		sendto(*game::ip_socket, static_cast<const char*>(data), static_cast<int>(length), 0,
		       reinterpret_cast<const sockaddr*>(&to), sizeof(to));
	}

	void send_data(const game::netadr_t& address, const std::string& data)
	{
		send_data(address, data.data(), data.size());
	}

	game::netadr_t address_from_string(const std::string& address)
	{
		game::netadr_t addr{};
		addr.localNetID = game::NS_SERVER;

		if (!game::NET_StringToAdr(address.data(), &addr))
		{
			addr.type = game::NA_BAD;
			return addr;
		}

		if (addr.type == game::NA_IP)
		{
			addr.type = game::NA_RAWIP;
		}

		return addr;
	}

	game::netadr_t address_from_ip(const uint32_t ip, const uint16_t port)
	{
		game::netadr_t addr{};
		addr.localNetID = game::NS_SERVER;
		addr.type = game::NA_RAWIP;
		addr.port = port;
		addr.addr = ip;

		return addr;
	}

	bool are_addresses_equal(const game::netadr_t& a, const game::netadr_t& b)
	{
		if (a.type != b.type)
		{
			return false;
		}

		if (a.type != game::NA_RAWIP && a.type != game::NA_IP)
		{
			return true;
		}

		return a.port == b.port && a.addr == b.addr;
	}

	int net_sendpacket_stub(const game::netsrc_t sock, const int length, const char* data, const game::netadr_t* to)
	{
		//printf("Sending packet of size: %X\n", length);

		if (to->type != game::NA_RAWIP)
		{
			printf("NET_SendPacket: bad address type\n");
			return 0;
		}

		const auto s = *game::ip_socket;
		if (!s || sock > game::NS_MAXCLIENTS)
		{
			return 0;
		}

		sockaddr_in address{};
		address.sin_family = AF_INET;
		address.sin_port = htons(to->port);
		address.sin_addr.s_addr = htonl(((to->ipv4.c | ((to->ipv4.b | (to->ipv4.a << 8)) << 8)) << 8) | to->ipv4.d);

		const auto size = static_cast<size_t>(length);

		std::vector<char> buffer{};
		buffer.resize(size + 1);
		buffer[0] = static_cast<char>((static_cast<uint32_t>(sock) & 0xF) | ((to->localNetID & 0xF) << 4));
		memcpy(buffer.data() + 1, data, size);

		return sendto(s, buffer.data(), static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&address),
		              sizeof(address));
	}

	struct component final : component_interface
	{
		void post_unpack() override
		{

		}
	};
}

REGISTER_COMPONENT(network::component)
