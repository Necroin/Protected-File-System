#include "Catalog.h"
#include "../../File System/FileSystem.h"


bool Catalog::OpenCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	catalog = object;
	return true;
}

bool Catalog::CreateFileCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	auto d_t = SystemObject::get_current_date_and_time();
	std::string name;
	std::cout << "Enter the name of file --> ";
	std::cin >> name;
	if (!static_cast<Catalog*>(catalog)->find(name)) {
		static_cast<Catalog*>(object)->add_object(new CommonFile(catalog,user, d_t.first, d_t.second, name));
		std::cout << "File added" << std::endl;
	}
	else {
		std::cout << "That name is already taken" << std::endl;
		return false;
	}
	return true;
}

bool Catalog::CreateCatalogCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	auto d_t = SystemObject::get_current_date_and_time();
	std::string name;
	std::cout << "Enter the name of catalog --> ";
	std::cin >> name;
	if (!static_cast<Catalog*>(catalog)->find(name)) {
		static_cast<Catalog*>(object)->add_object(new Catalog(static_cast<Catalog*>(object), user, d_t.first, d_t.second, name));
		std::cout << "Catalog added" << std::endl;
	}
	else {
		std::cout << "That name is already taken" << std::endl;
		return false;
	}
	return true;
}

bool Catalog::BackUpCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	catalog = catalog->get_parent();
	return true;
}

bool Catalog::LogOutCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	user = nullptr;
	return true;
}

bool Catalog::PutInCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	if (FileSystem::buffer_command && FileSystem::buffer_object) {
		FileSystem::buffer_command->execute(catalog, FileSystem::buffer_object, user);
	}
	return true;
}