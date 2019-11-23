#include "EncryptedFile.h"

EncryptedFile::EncryptedFile(User* owner, Date date, Time time, std::string path, std::string name) : CommonFile(owner, date, time, path, name)
{
}

EncryptedFile::~EncryptedFile()
{
}

void EncryptedFile::Destroy_all_commands()
{
}

EncryptedFile::Command* EncryptedFile::Open(const User& user)
{
	return nullptr;
}

EncryptedFile::Command* EncryptedFile::Copy(const User& user)
{
	return nullptr;
}

EncryptedFile::Command* EncryptedFile::Move(const User& user)
{
	return nullptr;
}

EncryptedFile::Command* EncryptedFile::Delete(const User& user)
{
	return nullptr;
}

EncryptedFile::Command* EncryptedFile::Decrypt(const User& user)
{
	return nullptr;
}

void EncryptedFile::File_Input(std::ifstream& fin)
{
	std::istream& in = fin;
	SystemObject::File_Input(fin);
}

void EncryptedFile::File_Output(std::ofstream& fout) const
{
	std::ostream& out = fout;
	out << "EncryptedFile" << " ";
	SystemObject::File_Output(fout);
}



const std::vector<std::string>& EncryptedFile::get_actions_list() const
{
	return actions_list;
}

EncryptedFile::Command* EncryptedFile::get_command(size_t index, const User& user)
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

void EncryptedFile::Show()
{
	SystemObject::Show();
	std::cout << "Encrypted file : " << _name << std::endl;
}
