#include "User.h"
#include "../System Object/SystemObject.h"

User::User(std::string name) : _name(name)
{
	_id = last_free_id++;
}

User::User() {}

size_t User::getID() const
{
	return _id;
}

const std::string& User::get_name() const
{
	return _name;
}