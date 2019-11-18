#pragma once
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_
#include "../System Object/Catalog/Catalog.h"
#include "../Contextual Menu/ContextualMenu.h"

class FileSystem
{
private:
	Catalog* root_catalog = nullptr;
	SystemObject* cur_catalog = nullptr;
	bool _active = true;

	std::vector<User*> _users;
	User* cur_user = nullptr;


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
	FileSystem(std::string path, std::string name);
	~FileSystem();

	void start();
	void log_on();
	void run();
	void stop();
};

#endif
