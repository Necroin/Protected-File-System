#include "User.h"
#include "../System Object/SystemObject.h"

User::User(std::string name) : _name(name)
{

}

User::User()
{
}

size_t User::getID() const
{
	return ID;
}

const std::string& User::get_name() const
{
	return _name;
}

bool User::can_read()
{
	return access._read;
}

bool User::can_write()
{
	return access._write;
}

bool User::can_run()
{
	return access._run;
}