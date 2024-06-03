#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace gamesettings
{
	namespace
	{
		// <name, path>
		std::unordered_map<std::string, std::string> game_settings_files;

		std::string get_game_settings_name(const std::vector<std::string>& sub_strings)
		{
			if (sub_strings.size() > 2)
			{
				return sub_strings[sub_strings.size() - 2] + '/' + sub_strings[sub_strings.size() - 1];
			}

			return {};
		}

		std::string get_game_settings_path(const std::string& name)
		{
			const auto itr = game_settings_files.find(name);
			return (itr == game_settings_files.end()) ? std::string() : itr->second;
		}

		void search_game_settings_folder(const std::string& game_settings_dir)
		{
			if (!utils::io::directory_exists(game_settings_dir))
			{
				return;
			}

			const auto files = utils::io::list_files(game_settings_dir, true);

			for (const auto& path : files)
			{
				if (!std::filesystem::is_directory(path))
				{
					auto sub_strings = utils::string::split(path.generic_string(), '/');
					game_settings_files.insert_or_assign(get_game_settings_name(sub_strings), path.generic_string());
				}
			}
		}

		bool has_game_settings_file_on_disk(const char* path)
		{
			if (!path)
			{
				return false;
			}

			const auto sub_strings = utils::string::split(path, '/');
			const auto game_settings_name = get_game_settings_name(sub_strings);

			return !get_game_settings_path(game_settings_name).empty();
		}

		void cmd_exec_stub(utils::hook::assembler& a)
		{
			const auto exec_from_fastfile = a.newLabel();
			const auto exec_from_disk = a.newLabel();

			a.pushad64();

			a.mov(rcx, r10);
			a.call_aligned(has_game_settings_file_on_disk);
			a.cmp(rax, 1);
;
			a.popad64();

			a.jnz(exec_from_fastfile);

			a.bind(exec_from_disk);
			a.jmp(0x1420ED087_g);

			a.bind(exec_from_fastfile);
			a.lea(rdx, ptr(rsp, 0x40));
			a.jmp(0x1420ED007_g);
		}

		int read_file_stub(const char* qpath, void** buffer)
		{
			const auto sub_strings = utils::string::split(qpath, '/');
			const auto game_settings_name = get_game_settings_name(sub_strings);

			std::string gamesettings_data;
			utils::io::read_file(get_game_settings_path(game_settings_name), &gamesettings_data);

			if (!gamesettings_data.empty())
			{
				++(*game::fs_loadStack);

				auto len = static_cast<int>(gamesettings_data.length());
				auto buf = game::FS_AllocMem(len + 1);

				*buffer = buf;
				gamesettings_data.copy(reinterpret_cast<char*>(*buffer), len);
				buf[len] = '\0';

				return len;
			}

			return utils::hook::invoke<int>(0x1422A48D0_g, qpath, buffer);
		}

		void search_gamesettings_files_on_disk()
		{
			const utils::nt::library host{};

			search_game_settings_folder((game::get_appdata_path() / "data/gamesettings").string());
			search_game_settings_folder((host.get_folder() / "boiii/gamesettings").string());
		}
	}

	struct component final : component_interface
	{
		void post_unpack() override
		{
			search_gamesettings_files_on_disk();

			utils::hook::call(0x1420ED0A1_g, read_file_stub);
			utils::hook::jump(0x1420ED002_g, utils::hook::assemble(cmd_exec_stub));
		}
	};
};

REGISTER_COMPONENT(gamesettings::component)
