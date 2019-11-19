#include "FileSystem.h"

FileSystem::FileSystem(const char* paths) {
	auto t = SystemObject::get_current_date_and_time();
	root_catalog = new Catalog(nullptr, t.first, t.second, "", "\\");
	cur_catalog = root_catalog;

	std::ifstream Paths_file(paths);
	std::getline(Paths_file, Data_file_path);
	std::getline(Paths_file, FreeDataBlocks_file_path);
	std::getline(Paths_file, Descriptors_file_path);
	std::getline(Paths_file, Users_file_path);
	Paths_file.close();
}

FileSystem::~FileSystem()
{
	delete root_catalog;
	for (size_t i = 0; i < _users.size(); i++)
	{
		delete _users[i];
	}
	Catalog::Destroy_all_commands();
	CommonFile::Destroy_all_commands();
	EncryptedFile::Destroy_all_commands();
	SystemObject::Destroy_all_commands();
}

void FileSystem::start()
{
	load_users();
}

void FileSystem::run()
{
	_active = true;
	while (_active)
	{
		if (cur_user) {
			cur_catalog->Show();
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
}
