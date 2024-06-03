#pragma once

#include "structs.hpp"
#include <utils/nt.hpp>

namespace arxan::detail
{
	void set_address_to_call(const void* address);
	extern void* callstack_proxy_addr;
}

namespace game
{
	size_t get_base();

	void show_error(const std::string& text, const std::string& title = "Error");

	inline size_t relocate(const size_t val)
	{
		if (!val) return 0;

		const auto base = get_base();
		return base + (val - 0x140000000);
	}

	inline size_t derelocate(const size_t val)
	{
		if (!val) return 0;

		const auto base = get_base();
		return (val - base) + 0x140000000;
	}

	inline size_t derelocate(const void* val)
	{
		return derelocate(reinterpret_cast<size_t>(val));
	}

	template <typename T>
	class base_symbol
	{
	public:
		base_symbol(const size_t address)
			: address_(address)
		{
		}

		T* get() const
		{
			return reinterpret_cast<T*>(game::relocate(this->address_));
		}

		operator T*() const
		{
			return this->get();
		}

		T* operator->() const
		{
			return this->get();
		}

	private:
		size_t address_{};
	};

	template <typename T>
	struct symbol : base_symbol<T>
	{
		using base_symbol<T>::base_symbol;
	};

	template <typename T, typename... Args>
	struct symbol<T(Args...)> : base_symbol<T(Args...)>
	{
		using func_type = T(Args...);

		using base_symbol<func_type>::base_symbol;

		T call_safe(Args... args)
		{
			arxan::detail::set_address_to_call(this->get());
			return static_cast<func_type*>(arxan::detail::callstack_proxy_addr)(args...);
		}
	};

	std::filesystem::path get_appdata_path();
}

inline size_t operator"" _g(const size_t val)
{
	return game::relocate(val);
}

#include "structs.hpp"
#include "symbols.hpp"
