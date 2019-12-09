#include "FileSystem.h"
#include "../WinhFunc/WinhFunc.h"

FileSystem::FileSystem(const char* paths) {
	//auto t = SystemObject::get_current_date_and_time();
	//root_catalog = new Catalog(nullptr, t.first, t.second, "", "\\");
	root_catalog = new Catalog(nullptr);
	root_catalog->set_name("\\");
	cur_catalog = root_catalog;

	std::ifstream Paths_file(paths);
	std::getline(Paths_file, Data_file_path);
	std::getline(Paths_file, FreeDataBlocks_file_path);
	std::getline(Paths_file, Descriptors_file_path);
	std::getline(Paths_file, Users_file_path);
	Paths_file.close();

	SystemObject::Init_all_commands();
	Catalog::Init_all_commands();
	CommonFile::Init_all_commands();
	EncryptedFile::Init_all_commands();
}

FileSystem::~FileSystem()
{
	delete root_catalog;
	for (size_t i = 0; i < _users.size(); i++)
	{
		delete _users[i];
	}
	SystemObject::Destroy_all_commands();
	Catalog::Destroy_all_commands();
	CommonFile::Destroy_all_commands();
	EncryptedFile::Destroy_all_commands();
}

void FileSystem::start()
{
	load_users();
	load_descriptors();
	CommonFile::load_free_blocks(FreeDataBlocks_file_path);
	CommonFile::open_data_file(Data_file_path);
}

void FileSystem::run()
{
	_active = true;
	while (_active)
	{
		if (cur_user) {
			cur_catalog->Show();
			Show_buffer();
			size_t choise = static_cast<Catalog*>(cur_catalog)->Wait();
			if (choise) {
				SystemObject* object = (*static_cast<Catalog*>(cur_catalog))[choise - 1];
				ContextualMenu object_menu(*object, *cur_user);
				object_menu.Show();
				typename SystemObject::Command* command = object_menu.Choose();
				command->execute(cur_catalog, object, cur_user);
			}
			else {
				static_cast<Catalog*>(cur_catalog)->Show_in_actions();
				typename SystemObject::Command* command = static_cast<Catalog*>(cur_catalog)->Choose_in_actions(*cur_user);
				command->execute(cur_catalog, cur_catalog, cur_user);
			}
		}
		else {
			log_on();
		}
	}
}

void FileSystem::stop()
{
	save_users();
	CommonFile::save_free_blocks(FreeDataBlocks_file_path);
	save_descriptors();
	CommonFile::close_data_file();
}


void FileSystem::Show_buffer()
{
	static const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	static const size_t x_pos = 60;
	static const size_t y_pos = 2;
	size_t command_mes_offset = 15;
	auto cur_cursor_coords = GetConsoleCursorPosition(hConsole);
	gotoxy(hConsole,x_pos, y_pos);
	std::cout << "Buffer :";
	gotoxy(hConsole, x_pos, y_pos + 1);
	if (buffer_object && buffer_command) {
		auto name = buffer_object->get_name();
		command_mes_offset = command_mes_offset > name.size() ? command_mes_offset : name.size() + 5;
		std::cout << name;
	}
	else {
		std::cout << "Nothing";
	}
	gotoxy(hConsole, x_pos + command_mes_offset, y_pos);
	std::cout << "Command :";
	gotoxy(hConsole, x_pos + command_mes_offset, y_pos + 1);
	std::cout << buffer_command_type;

	gotoxy(hConsole, cur_cursor_coords.X, cur_cursor_coords.Y);
}