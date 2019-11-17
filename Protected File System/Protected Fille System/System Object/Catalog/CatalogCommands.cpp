#include "Catalog.h"
#include "../../File System/FileSystem.h"

bool Catalog::OpenCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	catalog = object;
	return true;
}

bool Catalog::CopyCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	//implementation
	return true;
}

bool Catalog::DeleteCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	static_cast<Catalog*>(catalog)->erase(object);
	return true;
}

bool Catalog::CreateFileCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	auto d_t = SystemObject::get_current_date_and_time();
	std::string name;
	std::cout << "Enter the name of file --> ";
	std::cin >> name;
	if (!static_cast<Catalog*>(catalog)->find(name)) {
		std::string path = object->get_path();
		path.append(object->get_name());
		path.append("\\");
		static_cast<Catalog*>(object)->add_object(new CommonFile(nullptr, d_t.first, d_t.second, path, name));
		std::cout << "File added" << std::endl;
	}
	else {
		std::cout << "That name is already taken" << std::endl;
		return false;
	}
	return true;
}

bool Catalog::CreateCatalogCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	auto d_t = SystemObject::get_current_date_and_time();
	std::string name;
	std::cout << "Enter the name of catalog --> ";
	std::cin >> name;
	if (!static_cast<Catalog*>(catalog)->find(name)) {
		std::string path = object->get_path();
		path.append(object->get_name());
		path.append("\\");
		static_cast<Catalog*>(object)->add_object(new Catalog(static_cast<Catalog*>(object), d_t.first, d_t.second, path, name));
		std::cout << "Catalog added" << std::endl;
	}
	else {
		std::cout << "That name is already taken" << std::endl;
		return false;
	}
	return true;
}

bool Catalog::BackUpCommand::execute(SystemObject*& catalog, SystemObject*& object) {
	catalog = static_cast<Catalog*>(catalog)->get_parent();
	return true;
}