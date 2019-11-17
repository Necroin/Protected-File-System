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
public:
	FileSystem(std::string path, std::string name);
	~FileSystem();

	void start();
	void log_on();
	void run();
	void stop();
};

#endif
