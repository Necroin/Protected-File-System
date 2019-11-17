#pragma once
#ifndef _CONTEXTUALMENU_H_
#define _CONTEXTUALMENU_H_
#include "../System Object/SystemObject.h"

class ContextualMenu {
private:
	SystemObject& _object;
	User& _user;
public:
	ContextualMenu(SystemObject& object, User& user) :
		_object(object),
		_user(user)
	{}
	void Show();
	SystemObject::Command* Choose();
};
#endif
