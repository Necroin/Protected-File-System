#include "CommonFile.h"
#include "../../../User/Table/UsersTable.h"
#include "../../../Input/Input.h"
#include "../../../WinhFunc/WinhFunc.h"
#include <conio.h>


CommonFile::CommonFile(SystemObject* catalog, User* owner, Date date, Time time, std::string name) : 
	SystemObject(catalog, owner, date, time, name)
{}

CommonFile::CommonFile(SystemObject* catalog) : 
	SystemObject(catalog)
{}

CommonFile::~CommonFile()
{
}

void CommonFile::Destroy_all_commands()
{
	delete open_command;
	delete encrypt_command;
}

void CommonFile::Init_all_commands()
{
	open_command    = new OpenCommand;
	encrypt_command = new EncryptCommand;
}

size_t CommonFile::get_free_block()
{
	size_t block = free_bloks.top();
	free_bloks.pop();
	if (free_bloks.empty()) { free_bloks.push(block + 1); }
	return block;
}

CommonFile::Command* CommonFile::Open(const User& user)
{
	return open_command;
}

CommonFile::Command* CommonFile::Encrypt(const User& user)
{
	return encrypt_command;
}

CommonFile::Command* CommonFile::Read(const User& user)
{
	for (size_t i = 0; i < _main.size(); ++i)
	{
		std::cout << _main[i].show(data);
	}
	_getch();
	return null_command;
}

void CommonFile::File_Input(std::ifstream& fin)
{
	std::istream& in = fin;
	SystemObject::File_Input(fin);
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

bool CommonFile::open_data_file(std::string path)
{
	data.open(path);
	return true;
}

bool CommonFile::close_data_file()
{
	data.close();
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

std::vector<Stream>& CommonFile::load_data()
{
	for (size_t i = 0; i < _main.size(); ++i)
	{
		_main[i].load(data);
	}
	return _main;
}

void CommonFile::save_data(const std::string new_data)
{
	size_t new_data_size = new_data.size();
	size_t symbols_read = 0;
	while (symbols_read < new_data_size)
	{
		/*if (!_main.empty()) {
			size_t last_buffer_size = _main[_main.size() - 1].get_buffer().size();
			if (last_buffer_size < Stream::block_size) {
				auto add_str = new_data.substr(symbols_read, Stream::block_size - last_buffer_size);
				size_t add_str_size = add_str.size();
				_main[_main.size() - 1].add_to_buffer(add_str);
				symbols_read += add_str_size;
				continue;
			}
		}*/

		_main.push_back(Stream(get_free_block(), new_data.substr(symbols_read, Stream::block_size)));
		symbols_read += Stream::block_size;
	}

	save_data();
}

void CommonFile::save_data()
{
	for (size_t i = 0; i < _main.size(); ++i)
	{
		_main[i].save(data);
	}
}

void CommonFile::delete_data()
{
	SystemObject::delete_data();
	for (size_t i = 0; i < _main.size(); ++i)
	{
		free_bloks.push(_main[i].get_offset());
	}
}

SystemObject* CommonFile::clone(SystemObject* new_parent, User* new_owner)
{
	auto date_and_time = get_current_date_and_time();
	CommonFile* copy_file = new CommonFile(new_parent, new_owner, date_and_time.first, date_and_time.second, _name);
	load_data();
	for (size_t i = 0; i < _main.size(); ++i)
	{
		copy_file->_main.push_back(Stream(get_free_block(), _main[i].get_buffer()));
	}
	save_data();
	copy_file->save_data();
	return copy_file;
}

void CommonFile::show_self(size_t offset)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	auto coords = GetConsoleCursorPosition(hConsole);
	gotoxy(hConsole, offset, coords.Y);
	std::cout << "(" << _type << ") " << _name << std::endl;
}
