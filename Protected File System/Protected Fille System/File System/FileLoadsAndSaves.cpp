#include "FileSystem.h"

void FileSystem::load_users() 
{
	std::ifstream Users(Users_file_path);
	User user;
	Users >> User::last_free_id;
	while (Users >> user)
	{
		_users.push_back(new User(user));
	}
	Users.close();
}

void FileSystem::save_users() 
{
	std::ofstream Users(Users_file_path,std::ios_base::trunc);
	Users << User::last_free_id << std::endl;
	size_t size = _users.size();
	for (size_t i = 0; i < size; i++)
	{
		Users << *(_users[i]) << std::endl;
	}
	Users.close();
}

void FileSystem::load_descriptors()
{
}

void FileSystem::save_descriptors()
{
	std::ofstream Descriptors(Descriptors_file_path,std::ios_base::trunc);
	Descriptors << *root_catalog;
	Descriptors.close();
}
