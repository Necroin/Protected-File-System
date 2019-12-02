#include "EncryptedFile.h"
#include "../../../User/Table/UsersTable.h"
#include "../../../Input/Input.h"

EncryptedFile::EncryptedFile(SystemObject* catalog, User* owner, Date date, Time time, std::string name) : CommonFile(catalog, owner, date, time, name)
{}

EncryptedFile::EncryptedFile(SystemObject* catalog) :
	CommonFile(catalog)
{}

EncryptedFile::~EncryptedFile()
{
}

void EncryptedFile::Destroy_all_commands()
{
}

void EncryptedFile::Init_all_commands()
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
	// Считывание ID владельца
	ID owner_id;
	in >> owner_id;
	// Поиск владельца по ID
	_owner = UsersTable::find(owner_id);
	if (_owner) {
		_owner->add_file(this);
	}
	// Считывание количства потоков
	size_t streams_count, stream_offset;
	// Main streams
	in >> streams_count;
	for (size_t i = 0; i < streams_count; ++i)
	{
		in >> stream_offset;
		_main.push_back(Stream(stream_offset));
	}
	// Symkey streams
	in >> streams_count;
	for (size_t i = 0; i < streams_count; ++i)
	{
		in >> stream_offset;
		_symkey.push_back(Stream(stream_offset));
	}
}

void EncryptedFile::File_Output(std::ofstream& fout) const
{
	std::ostream& out = fout;
	out << "EncryptedFile" << " ";
	SystemObject::File_Output(fout);
	// Вывод ID владельца
	out << _owner->getID() << std::endl;
	// Определение и вывод количества потоков _main
	size_t streams_size = _main.size();
	// 
	fout << streams_size;
	for (size_t i = 0; i < streams_size; ++i)
	{
		fout << " ";
		fout << _main[i].get_offset();
	}
	fout << std::endl;
	// Определение и вывод количества потоков _symkey
	streams_size = _symkey.size();
	// 
	fout << streams_size;
	for (size_t i = 0; i < streams_size; ++i)
	{
		fout << " ";
		fout << _symkey[i].get_offset();
	}
	// END
	out << std::endl;
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

const std::string& EncryptedFile::get_object_type() const
{
	return _type;
}
