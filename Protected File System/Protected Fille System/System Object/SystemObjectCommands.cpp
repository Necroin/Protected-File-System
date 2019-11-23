#include "SystemObject.h"
#include "Catalog/Catalog.h"
#include "../WinhFunc/WinhFunc.h"
#include <conio.h>

bool SystemObject::Command::execute(SystemObject*& catalog, SystemObject*& object, User*& user) { return true; }

bool SystemObject::ErrorMessageCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	std::cout << _message << std::endl;
	return true;
}

//////////////////////////////////////////////////////////////// RENAME ////////////////////////////////////////////////////////////////
bool SystemObject::RenameCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////// MOVE /////////////////////////////////////////////////////////////////

Catalog* get_root(Catalog* catalog) {
	while (catalog->have_parent())
	{
		catalog = catalog->get_parent();
	}
	return catalog;
}

std::string choose_path(Catalog* cur_catalog)
{
	cur_catalog = get_root(cur_catalog);
	std::cout << "Enter path --> ";
	std::string path;
	std::string path_element;
	char letter;
	HANDLE  outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	const COORD cursor_start_coordinates = GetConsoleCursorPosition(outputHandle);
	COORD cursor_pos = cursor_start_coordinates;
	COORD last_valid_cursor_pos = cursor_start_coordinates;
	while (letter = getchar()) {
		if (letter == '\b') {
			cursor_pos.X -= 1;
		}
		if (letter == '\b') {

		}
	}



	return path;
};

bool SystemObject::MoveCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	std::string seek_path = choose_path(static_cast<Catalog*>(catalog));
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////