#include "ContextualMenu.h"
#include "../Input/Input.h"

void ContextualMenu::Show()
{
	std::cout << "Actions with \"" << _object.get_name() << "\" :" << std::endl;
	auto actions = _object.get_actions_list();
	auto size = actions.size();
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << actions[i] << std::endl;
	}
}

SystemObject::Command* ContextualMenu::Choose()
{
	int index, size = _object.get_actions_list().size();
	std::cout << "Choose -->";
	get_object(std::cin, index,
		[size](int number) {
			if (number < 0 || number >= size) {
				std::cout << "Your input must be >= 0 and <= " << size-1 << std::endl;
				return 1;
			}
			return 0;
		});
	return _object.get_command(index, _user);
}
