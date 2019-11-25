#pragma once
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_
#include "../System Object/Catalog/Catalog.h"
#include "../Contextual Menu/ContextualMenu.h"
#include "../User/Table/UsersTable.h"
#include <fstream>

class FileSystem
{
private:
	Catalog* root_catalog = nullptr;
	SystemObject* cur_catalog = nullptr;
	bool _active = true;

	UsersTable _users;
	User* cur_user = nullptr;


	std::string Data_file_path;
	std::string FreeDataBlocks_file_path;
	std::string Descriptors_file_path;
	std::string Users_file_path;


	void load_users();
	void save_users();

	void load_descriptors();
	void save_descriptors();

	inline static const std::vector<std::string> log_on_list = {
		"0.Close",  
		"1.Sign in",
		"2.Sign up"
	};

	typedef void (FileSystem::* fptr)();
	fptr log_on_actions[3] = {
		&FileSystem::Close,
		&FileSystem::Sign_in,
		&FileSystem::Sign_up
	};

	void Close();
	void Sign_in();
	void Sign_up();
public:
	FileSystem(const char* file_with_paths);
	~FileSystem();

	void start();
	void log_on();
	void run();
	void stop();
};

#endif
