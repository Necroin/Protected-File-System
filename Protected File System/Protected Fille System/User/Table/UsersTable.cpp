#include "UsersTable.h"
#include "../../Input/Input.h"
#include "../../System Object/SystemObject.h"
#include <conio.h>


User* UsersTable::find(const ID& id)
{
	User* find_user = nullptr;
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->getID() == id) {
			find_user = _users[i];
			break;
		}
	}
	return find_user;
}

const std::vector<User*>& UsersTable::get_all_users()
{
	return _users;
}

void UsersTable::add(User* user)
{
	_users.push_back(user);
}

void UsersTable::erase(const ID& id)
{
	std::vector<User*>::const_iterator it = _users.begin();
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->getID() == id) {
			_users.erase(it);
			break;
		}
		++it;
	}
}

size_t UsersTable::size()
{
	return _users.size();
}

User* UsersTable::operator[](size_t index)
{
	return _users[index];
}

void UsersTable::EditTable()
{
	typedef void (* fptr)();
	static constexpr fptr actions[2] = {
		&UsersTable::Edit_user,
		&UsersTable::Delete_user
	};
	int choise = 1;
	while (choise)
	{
		system("cls");
		std::cout << "Actions :" << std::endl;
		std::cout << "0.Cancel" << std::endl;
		std::cout << "1.Edit the user" << std::endl;
		std::cout << "2.Delete the user" << std::endl;

		std::cout << "Choose --> ";
		get_object(std::cin, choise,
			[](int number) -> int
			{
				if (number < 0 || number >= 2) {
					std::cout << "Your input must be >= " << 0 << " and <=" << 2 << std::endl;
					return 1;
				}
				return 0;
			});

		if (choise) {
			(*actions[choise - 1])();
		}
	}
}

void UsersTable::Edit_user()
{
	std::cout << "0.Cancel" << std::endl;
	std::cout << "Users :" << std::endl;
	auto size = _users.size();
	int choise;
	for (size_t i = 2; i < size; ++i)
	{
		std::cout << i + 1 << "." << _users[i]->get_name() << std::endl;
	}
	std::cout << "Choose the user --> ";
	get_object(std::cin, choise,
		[size](int number) -> int
		{
			if (number < 0 || number >= size - 2) {
				std::cout << "Your input must be >= " << 0 << " and <=" << size - 2 << std::endl;
				return 1;
			}
			return 0;
		});
	if (choise) {
		int index = choise + 1;
		auto& user = _users[index];
		std::cout << "What you want to edit?";
		std::cout << "0.Cancel" << std::endl;
		std::cout << "1.Edit name" << std::endl;
		std::cout << "2.Edit ID" << std::endl;
		std::cout << "Choose --> ";
		get_object(std::cin, choise,
			[](int number) -> int
			{
				if (number < 0 || number >= 2) {
					std::cout << "Your input must be >= " << 0 << " and <=" << 2 << std::endl;
					return 1;
				}
				return 0;
			});
		if (choise) {
			if(choise == 1){
				std::string new_name;
				std::cout << "Enter new name -->";
				user->_name = new_name;
				std::cout << "Name changed";
				_getch();
			}
			if (choise == 2) {
				ID new_id;
				get_object(std::cin, new_id,
					[](ID id) -> int
					{
						if (id == 0 || id == 1) {
							return 1;
						}
						return 0;
					});
				std::cout << "ID changed";
				_getch();
			}
		}
	}
}

void UsersTable::Delete_user()
{
	std::cout << "0.Cancel" << std::endl;
	std::cout << "Users :" << std::endl;
	auto size = _users.size();
	int choise;
	for (size_t i = 2; i < size; ++i)
	{
		std::cout << i + 1 << "." << _users[i]->get_name() << std::endl;
	}
	std::cout << "Choose the user --> ";
	get_object(std::cin, choise,
		[size](int number) -> int
		{
			if (number < 0 || number >= size - 2) {
				std::cout << "Your input must be >= " << 0 << " and <=" << size - 2 << std::endl;
				return 1;
			}
			return 0;
		});
	if (choise) {
		int index = choise + 1;
		auto& objects = _users[index]->_objects;
		size_t objects_size = objects.size();
		for (size_t i = 0; i < objects_size; i++)
		{
			objects[i]->_owner = _users[1];
		}
		_users.erase(_users.begin() + index);
	}
}
