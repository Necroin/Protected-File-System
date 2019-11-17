#include "FileSystem.h"

FileSystem::FileSystem(std::string path, std::string name) {
	auto t = SystemObject::get_current_date_and_time();
	root_catalog = new Catalog(nullptr, t.first, t.second, path, name);
	cur_catalog = root_catalog;
}

FileSystem::~FileSystem()
{
	delete root_catalog;
}

void FileSystem::start()
{

}

void FileSystem::log_on()
{
	std::cout
		<< "0.Close"   << std::endl
		<< "1.Sign in" << std::endl
		<< "2.Sign up" << std::endl;

}

void FileSystem::run()
{
	while (_active)
	{
		cur_catalog->Show();
		size_t choise = static_cast<Catalog*>(cur_catalog)->Wait();
		if (choise) {
			SystemObject* object = (*static_cast<Catalog*>(cur_catalog))[choise - 1];
			ContextualMenu object_menu(*object, *cur_user);
			object_menu.Show();
			SystemObject::Command* command = object_menu.Choose();
			command->execute(cur_catalog, object);
		}
		else {
			static_cast<Catalog*>(cur_catalog)->Show_in_actions();
			SystemObject::Command* command = static_cast<Catalog*>(cur_catalog)->Choose_in_actions(*cur_user);
			command->execute(cur_catalog, cur_catalog);
		}
	}
}

void FileSystem::stop()
{
}
