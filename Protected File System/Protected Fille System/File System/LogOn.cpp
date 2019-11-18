#include "FileSystem.h"
#include "../Input/Input.h"

void FileSystem::log_on()
{
	cur_catalog = root_catalog;
	while (!cur_user && _active) {
		system("cls");
		int choise;
		size_t size = log_on_list.size();
		for (size_t i = 0; i < size; i++)
		{
			std::cout << log_on_list[i] << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Chooose the options --> ";
		get_number(std::cin, choise,
			[size](int number) -> int
			{
				if (number < 0 || number >= size) {
					std::cout << "Your input must be >= " << 0 << " and <=" << size - 1 << std::endl;
					return 1;
				}
				return 0;
			}
		);
		(this->*log_on_actions[choise])();
	}
}

void FileSystem::Close()
{
	_active = false;
}

void FileSystem::Sign_in()
{
	system("cls");
	int choise;
	size_t users_count = _users.size();
	std::cout << "0.Back" << std::endl;
	for (size_t i = 0; i < users_count; ++i)
	{
		std::cout << i + 1 << "." << _users[i]->get_name() << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Chooose profile --> ";
	get_number(std::cin, choise,
		[users_count](int number) -> int
		{
			if (number<0 || number > users_count) {
				std::cout << "Your input must be >= " << 0 << " and <=" << users_count << std::endl;
				return 1;
			}
			return 0;
		}
	);
	if (choise) {
		// TODO: check password
		cur_user = _users[choise - 1];
	}
}

void FileSystem::Sign_up()
{
	system("cls");
	std::string name;
	std::cout << "Enter name of new profile --> ";
	std::cin >> name;
	_users.push_back(new User(name));
}