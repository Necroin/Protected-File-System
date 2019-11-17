#include "CommonFile.h"

CommonFile::Command* CommonFile::Open(const User& user)
{
	return nullptr;
}

CommonFile::Command* CommonFile::Copy(const User& user)
{
	return nullptr;
}

CommonFile::Command* CommonFile::Move(const User& user)
{
	return nullptr;
}

CommonFile::Command* CommonFile::Delete(const User& user)
{
	return nullptr;
}

CommonFile::Command* CommonFile::Encrypt(const User& user)
{
	return nullptr;
}

CommonFile::CommonFile(User* owner,Date date, Time time, std::string path, std::string name) : SystemObject(date, time, path, name), _owner(owner)
{

}

const std::vector<std::string>& CommonFile::get_actions_list() const
{
	return actions_list;
}

CommonFile::Command* CommonFile::get_command(size_t index, const User& user)
{
	try
	{
		return (this->*actions[index])(user);
	}
	catch (const std::exception & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return null_command;
}

void CommonFile::Show()
{
	SystemObject::Show();
	std::cout << "Common file : " << _name << std::endl;
}
