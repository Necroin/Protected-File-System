#pragma once
#ifndef _USER_H_
#define _USER_H_
#include <string>
#include <vector>
#include <iostream>

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
	User();
	inline static const size_t max_name_lenght = 20;
	size_t getID() const;
	const std::string& get_name() const;
	bool can_read();
	bool can_write();
	bool can_run();

	friend std::istream& operator>>(std::istream& in, User& user);
	friend std::ostream& operator<<(std::ostream& out, const User& user);
};
#endif

inline std::istream& operator>>(std::istream& in, User& user)
{
	in >> user._name;
	return in;
}

inline std::ostream& operator<<(std::ostream& out, const User& user)
{
	out << user._name;
	return out;
}
