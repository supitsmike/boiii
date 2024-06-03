#pragma once

enum class component_priority
{
	min = 0,
	// must run after the steam_proxy
	name,
	// must run after the updater
	steam_proxy,
	updater,
	// must have the highest priority
	arxan,
};

struct component_interface
{
	virtual ~component_interface() = default;

	virtual void post_load()
	{
	}

	virtual void pre_destroy()
	{
	}

	virtual void post_unpack()
	{
	}

	virtual component_priority priority() const
	{
		return component_priority::min;
	}
};
