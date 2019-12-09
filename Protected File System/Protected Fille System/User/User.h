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

	friend std::ifstream& operator>>(std::ifstream& fin, AccessSpecifiers& access);
	friend std::ofstream& operator<<(std::ofstream& fout, const AccessSpecifiers& access);
};



class SystemObject;

using ID = size_t;

class User 
{
private:
	ID _id;
	std::string _name;
	std::vector<SystemObject*> _objects;
public:
	User(std::string name);
	User();
	inline static const size_t max_name_lenght = 20;
	inline static ID last_free_id;
	size_t getID() const;
	const std::string& get_name() const;
	void add_object(SystemObject* object);

	friend std::ifstream& operator>>(std::ifstream& fin, User& user);
	friend std::ofstream& operator<<(std::ofstream& fout, const User& user);
};
#endif

inline std::ifstream& operator>>(std::ifstream& fin, AccessSpecifiers& access)
{
	fin >> access._read >> access._write >> access._run;
	return fin;
}

inline std::ofstream& operator<<(std::ofstream& fout, const AccessSpecifiers& access)
{
	fout << access._read << access._write << access._run;
	return fout;
}

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
