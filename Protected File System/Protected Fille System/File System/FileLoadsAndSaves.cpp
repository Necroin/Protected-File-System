#include "FileSystem.h"

void FileSystem::load_users() 
{
	std::ifstream Users(Users_file_path);
	User user;
	while (Users >> user)
	{
		_users.push_back(new User(user));
	}
	Users.close();
}

void FileSystem::save_users() 
{
	std::ofstream Users(Users_file_path,std::ios_base::trunc);
	size_t size = _users.size();
	for (size_t i = 0; i < size; i++)
	{
		Users << *(_users[i]) << std::endl;
	}
	Users.close();
}