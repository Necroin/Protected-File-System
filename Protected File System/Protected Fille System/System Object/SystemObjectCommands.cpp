#include "SystemObject.h"
#include "Catalog/Catalog.h"


bool SystemObject::Command::execute(SystemObject*& catalog, SystemObject*& object) { return true; }

bool SystemObject::ErrorMessageCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	std::cout << _message << std::endl;
	return true;
}

bool SystemObject::RenameCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	std::string new_name;
	std::cout << "Enter new name --> ";
	std::cin >> new_name;
	if (!static_cast<Catalog*>(catalog)->find(new_name)) {
		object->set_name(new_name);
		std::cout << "Catalog renamed" << std::endl;
	}
	else {
		std::cout << "That name is already taken" << std::endl;
		return false;
	}
	return true;
}


bool SystemObject::MoveCommand::execute(SystemObject*& catalog, SystemObject*& object)
{
	//implementation
	return true;
}