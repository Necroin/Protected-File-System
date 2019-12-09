#include "FileSystem.h"
#include "../User/Table/UsersTable.h"

void FileSystem::load_users() 
{
	std::ifstream Users(Users_file_path);
	User user;
	Users >> User::last_free_id;
	while (Users >> user)
	{
		_users.add(new User(user));
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
	std::ifstream Descriptors(Descriptors_file_path);
	std::string root_type;
	if (Descriptors >> root_type && root_type == "Catalog") {
			Descriptors >> *root_catalog;
	}
	else {
		Reset();
	}
	Descriptors.close();
}

void FileSystem::save_descriptors()
{
	std::ofstream Descriptors(Descriptors_file_path,std::ios_base::trunc);
	Descriptors << *root_catalog;
	Descriptors.close();
}


void FileSystem::Reset() {
	delete root_catalog;
	auto date_and_time = SystemObject::get_current_date_and_time();
	root_catalog = new Catalog(nullptr,UsersTable::find(1), date_and_time.first, date_and_time.second, "\\");
	cur_catalog = root_catalog;
	std::ofstream(Descriptors_file_path, std::ios_base::trunc).close();
	std::ofstream(Data_file_path, std::ios_base::trunc).close();
	(std::ofstream(FreeDataBlocks_file_path, std::ios_base::trunc) << 0).close();
}
