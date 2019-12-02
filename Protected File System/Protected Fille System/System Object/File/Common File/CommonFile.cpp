#include "CommonFile.h"
#include "../../../User/Table/UsersTable.h"
#include "../../../Input/Input.h"


CommonFile::CommonFile(SystemObject* catalog, User* owner, Date date, Time time, std::string name) : 
	SystemObject(catalog, date, time, name), 
	_owner(owner)
{}

CommonFile::CommonFile(SystemObject* catalog) : 
	SystemObject(catalog)
{}

CommonFile::~CommonFile()
{
}

void CommonFile::Destroy_all_commands()
{
}

void CommonFile::Init_all_commands()
{
}

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

CommonFile::Command* CommonFile::Show_owner(const User& user)
{
	std::cout << _owner->get_name();
	std::string a;
	get_object(std::cin, a, [](std::string a) -> int
		{
			if (a != "") { return 1; }
			return 0;
		});
	return null_command;
}

void CommonFile::File_Input(std::ifstream& fin)
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
	in >> streams_count;
	for (size_t i = 0; i < streams_count; ++i)
	{
		in >> stream_offset;
		_main.push_back(Stream(stream_offset));
	}
}

void CommonFile::File_Output(std::ofstream& fout) const
{
	std::ostream& out = fout;
	// Выводе идентификатора обычного файла
	out << "CommonFile" << " ";
	// Стандартный вывод объекта
	SystemObject::File_Output(fout);
	// Вывод ID владельца
	out << _owner->getID() << std::endl;
	// Определение и вывод количества потоков main
	size_t streams_size = _main.size();
	fout << streams_size;
	for (size_t i = 0; i < streams_size; ++i)
	{
		fout << " ";
		fout << _main[i].get_offset();
	}
	// END
	out << std::endl;
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

const std::string& CommonFile::get_object_type() const
{
	return _type;
}

bool CommonFile::set_data_path(std::string path)
{
	data.open(path);
	return true;
}

bool CommonFile::load_free_blocks(std::string path)
{
	std::ifstream f_b(path);
	size_t block_number;
	while (f_b >> block_number)
	{
		free_bloks.push(block_number);
	}
	return true;
}

bool CommonFile::save_free_blocks(std::string path)
{
	std::ofstream f_b(path);
	while (!free_bloks.empty())
	{
		f_b << free_bloks.top() << std::endl;
		free_bloks.pop();
	}
	return true;
}
