#pragma once
#ifndef _USER_H_
#define _USER_H_
#include <string>
#include <vector>

struct AccessSpecifiers
{
	bool _read  = 0;
	bool _write = 0;
	bool _run   = 0;
};

class SystemObject;

class User 
{
private:
	inline static size_t ID;
	AccessSpecifiers access;
	std::string _name;
	std::vector<SystemObject*> _files;
public:
	User(std::string name);
	size_t getID() const;
	std::string get_name();
	bool can_read();
	bool can_write();
	bool can_run();
};
#endif 
