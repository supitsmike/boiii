#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "scheduler.hpp"

#include <game/game.hpp>
#include <game/utils.hpp>

#include <utils/hook.hpp>
#include <utils/io.hpp>

#include <mmeapi.h>

namespace client_patches
{
	namespace
	{
		const game::dvar_t* cl_yaw_speed;
		const game::dvar_t* cl_pitch_speed;

		void reduce_process_affinity()
		{
			const DWORD_PTR affinity = (1ULL << (std::min(std::thread::hardware_concurrency(), 4U))) - 1;
			SetProcessAffinityMask(GetCurrentProcess(), affinity);
		}

		void reset_process_affinity()
		{
			DWORD_PTR affinity_proc, affinity_sys;
			GetProcessAffinityMask(GetCurrentProcess(), &affinity_proc, &affinity_sys);
			SetProcessAffinityMask(GetCurrentProcess(), affinity_sys);
		}

		void fix_amd_cpu_stuttering()
		{
			scheduler::once([]
			{
				reduce_process_affinity();
				scheduler::once(reset_process_affinity, scheduler::pipeline::main, 1s);
			}, scheduler::pipeline::main);
		}

		float cl_key_state_yaw_speed_stub(void* key)
		{
			return game::CL_KeyState(key) * cl_yaw_speed->current.value.value;
		}

		float cl_key_state_pitch_speed_stub(void* key)
		{
			return game::CL_KeyState(key) * cl_pitch_speed->current.value.value;
		}
	}

	class component final : public component_interface
	{
	public:
		static_assert(offsetof(game::clientActive_t, viewangles) == 0xB8C8);

		void post_unpack() override
		{
			fix_amd_cpu_stuttering();

			cl_yaw_speed = game::register_dvar_float("cl_yawspeed", 140.0f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(),
			                                         game::DVAR_NONE, "Max yaw speed in degrees for game pad and keyboard");
			cl_pitch_speed = game::register_dvar_float("cl_pitchspeed", 140.0f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(),
			                                           game::DVAR_NONE, "Max pitch speed in degrees for game pad");
			// CL_AdjustAngles
			utils::hook::call(0x1412F3324_g, cl_key_state_yaw_speed_stub); // cl_yawspeed
			utils::hook::call(0x1412F3344_g, cl_key_state_yaw_speed_stub); // ^^

			utils::hook::call(0x1412F3380_g, cl_key_state_pitch_speed_stub); // cl_pitchspeed
			utils::hook::call(0x1412F33A1_g, cl_key_state_pitch_speed_stub); // ^^
		}
	};
}

REGISTER_COMPONENT(client_patches::component)
