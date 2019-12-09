#include "UsersTable.h"

User* UsersTable::find(const ID& id)
{
	User* find_user = nullptr;
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->getID() == id) {
			find_user = _users[i];
			break;
		}
	}
	return find_user;
}

const std::vector<User*>& UsersTable::get_all_users()
{
	return _users;
}

void UsersTable::add(User* user)
{
	_users.push_back(user);
}

void UsersTable::erase(const ID& id)
{
	std::vector<User*>::const_iterator it = _users.begin();
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i]->getID() == id) {
			_users.erase(it);
			break;
		}
		++it;
	}
}

size_t UsersTable::size()
{
	return _users.size();
}

User* UsersTable::operator[](size_t index)
{
	return _users[index];
}
