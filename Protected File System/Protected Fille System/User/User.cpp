#include "User.h"
#include "../System Object/SystemObject.h"

size_t User::getID() const
{
	return ID;
}

std::string User::get_name()
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
