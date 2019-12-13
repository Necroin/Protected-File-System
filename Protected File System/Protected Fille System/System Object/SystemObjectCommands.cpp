#include "SystemObject.h"
#include "Catalog/Catalog.h"
#include "../User/Table/UsersTable.h"
#include "../Input/Input.h"
#include <conio.h>

bool SystemObject::Command::execute(SystemObject*& catalog, SystemObject*& object, User*& user) { return true; }

bool SystemObject::ErrorMessageCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	std::cout << _message << std::endl;
	_getch();
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

bool SystemObject::MoveCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{

	static_cast<Catalog*>(catalog)->add_object(object);
	SystemObject* object_parrent = object->get_parent();
	static_cast<Catalog*>(object_parrent)->erase(object);
	object->_parent = catalog;
	SystemObject::reset_buffer();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool SystemObject::DeleteCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	object->delete_data();
	static_cast<Catalog*>(catalog)->erase(object);
	delete object;
	return true;
}

bool SystemObject::CopyCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	SystemObject* copy_object = object->clone(catalog,user);
	static_cast<Catalog*>(catalog)->add_object(copy_object);
	auto copy_name = copy_object->_name;
	copy_object->_name = copy_name.substr(0, copy_name.find("(Copy_"));
	copy_object->_name.append("(Copy_").append(std::to_string(object->copy_number++)).append(")");
	SystemObject::reset_buffer();
	return true;
}

bool SystemObject::ChangePermissionsCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user) { 
	std::cout << "Users with permissions :" << std::endl;
	std::vector<User*> users;
	size_t users_count = object->_users_access.size();
	for (size_t i = 0; i < users_count; ++i)
	{
		users.push_back(UsersTable::find(object->_users_access[i].first));
	}

	if (users.size()) {
		std::cout << "0.Cancel" << std::endl;
		std::cout << "Users to change :";
		for (size_t i = 0; i < users_count; ++i)
		{
			std::cout << i + 1 << "." << users[i]->get_name() << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Choose the user --> ";
		int choise = 1;
		get_object(std::cin, choise, [users_count](int number) -> int {
			if (number < 0 || number > users_count) {
				return 1;
			}
			return 0;
		});
		std::cout << std::endl;
		if (choise) {
			int index = choise - 1;
			std::cout << users[index]->get_name() << " rights :";
			AccessSpecifiers& access = object->_users_access[index].second;
			std::cout << "Read  : " << (access._read ? "true" : "false");
			std::cout << "Write : " << (access._write ? "true" : "false");
			std::cout << "Run   : " << (access._run ? "true" : "false");

			std::cout << "What do you want to change? ";
			std::string access_choise;
			get_object(std::cin, access_choise, [](std::string str) -> int {
				if (str != "Read" && str != "Write" && str != "Run") {
					return 1;
				}
				return 0;
				});

			std::string access_right;
			get_object(std::cin, access_right, [](std::string str) -> int {
				if (str != "true" && str != "false") {
					return 1;
				}
				return 0;
				});

			bool right_value = access_right == "true" ? true : false;

			if (access_choise == "Read") access._read = right_value;
			if (access_choise == "White") access._write = right_value;
			if (access_choise == "Run") access._run = right_value;
		}
	}
	else {
		std::cout << "There are no users to change";
		_getch();
	}

	return true; 
}

bool SystemObject::AddPermissionsCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user) { 
	auto& all_users = UsersTable::get_all_users();
	auto& users_access = object->_users_access;
	auto find_user = [&users_access](ID id) -> bool {
		for (size_t i = 0; i < users_access.size(); i++)
		{
			if (users_access[i].first == id) { return true; }
		}
		return false;
	};

	std::vector<User*> users;
	for (size_t i = 0; i < all_users.size(); ++i)
	{
		if (!find_user(all_users[i]->getID())) {
			users.push_back(all_users[i]);
		}
	}

	if (users.size()) {
		std::cout << "0.Cancel" << std::endl;
		std::cout << "Users to add :";
		auto users_count = users.size();
		for (size_t i = 0; i < users_count; ++i)
		{
			std::cout << i + 1 << "." << users[i]->get_name();
		}
		std::cout << "Choose the user -->";
		int choise;
		get_object(std::cin, choise, [users_count](int number) -> int {
			if (number < 0 || number > users_count) {
				return 1;
			}
			return 0;
		});
		std::cout << std::endl;
		if (choise) {
			size_t index = choise - 1;
			std::vector<bool> access(3);
			std::vector<std::string> rigths = { "Read : ", "Write : ", "Run : " };
			for (size_t i = 0; i < 3; ++i)
			{
				std::cout << rigths[i];
				std::string access_right;
				get_object(std::cin, access_right, [](std::string str) -> int {
					if (str != "true" && str != "false") {
						return 1;
					}
					return 0;
				});
				access[i] = access_right == "true" ? true : false;
			}
			users_access.push_back({ users[index]->getID(),AccessSpecifiers{access[0],access[1],access[2] } });
		}

	}
	else {
		std::cout << "There are no users to add";
		_getch();
	}

	return true;
}

bool SystemObject::DeletePermissionsCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user) { 
	std::cout << "Users with permissions :" << std::endl;
	std::vector<User*> users;
	auto& users_access = object->_users_access;
	size_t users_count = users_access.size();
	for (size_t i = 0; i < users_count; ++i)
	{
		users.push_back(UsersTable::find(object->_users_access[i].first));
	}

	if (users.size()) {
		std::cout << "0.Cancel" << std::endl;
		std::cout << "Users to delete :";
		for (size_t i = 0; i < users_count; ++i)
		{
			std::cout << i + 1 << "." << users[i]->get_name() << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Choose the user --> ";
		int choise = 1;
		get_object(std::cin, choise, [users_count](int number) -> int {
			if (number < 0 || number > users_count) {
				return 1;
			}
			return 0;
			});
		std::cout << std::endl;
		if (choise) {
			int index = choise - 1;
			users_access.erase(users_access.begin() + index);
		}
	}
	else {
		std::cout << "There are no users to delete";
		_getch();
	}

	return true; 
}
