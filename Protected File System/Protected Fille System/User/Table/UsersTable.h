#pragma once
#ifndef _USERSTABLE_H_
#define _USERSTABLE_H_
#include "../User.h"

class UsersTable {
private:
	inline static std::vector<User*> _users;

	static void Edit_user();
	static void Delete_user();

	
public:
	static User* find(const ID& id);
	static const std::vector<User*>& get_all_users();
	void add(User* user);
	void erase(const ID& id);
	size_t size();

	User* operator[](size_t index);

	static void EditTable();
};
#endif // !_USERSTABLE_H_
