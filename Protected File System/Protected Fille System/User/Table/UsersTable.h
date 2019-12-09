#pragma once
#ifndef _USERSTABLE_H_
#define _USERSTABLE_H_
#include "../User.h"

class UsersTable {
private:
	inline static std::vector<User*> _users;
public:
	static User* find(const ID& id);
	static const std::vector<User*>& get_all_users();
	void add(User* user);
	void erase(const ID& id);
	size_t size();

	User* operator[](size_t index);
};
#endif // !_USERSTABLE_H_
