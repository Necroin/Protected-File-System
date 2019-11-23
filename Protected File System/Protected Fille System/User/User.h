#pragma once
#ifndef _USER_H_
#define _USER_H_
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

struct AccessSpecifiers
{
	bool _read  = 0;
	bool _write = 0;
	bool _run   = 0;
};

class SystemObject;

using ID = size_t;

class User 
{
private:
	ID _id;
	std::string _name;
	std::vector<SystemObject*> _files;
public:
	User(std::string name);
	User();
	inline static const size_t max_name_lenght = 20;
	inline static ID last_free_id;
	size_t getID() const;
	const std::string& get_name() const;

	friend std::ifstream& operator>>(std::ifstream& fin, User& user);
	friend std::ofstream& operator<<(std::ofstream& fout, const User& user);
};
#endif

inline std::ifstream& operator>>(std::ifstream& fin, User& user)
{
	std::istream& in = fin;
	in >> user._name >> user._id;
	return fin;
}

inline std::ofstream& operator<<(std::ofstream& fout, const User& user)
{
	std::ostream& out = fout;
	out << user._name << " " << user._id;
	return fout;
}
