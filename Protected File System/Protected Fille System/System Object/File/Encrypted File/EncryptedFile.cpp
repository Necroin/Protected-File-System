#include "EncryptedFile.h"
#include "../../../User/Table/UsersTable.h"
#include "../../../Input/Input.h"
#include "../../../WinhFunc/WinhFunc.h"
#include <conio.h>

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
	delete open_command;
	delete decrypt_command;
}

void EncryptedFile::Init_all_commands()
{
	open_command = new OpenCommand;
	decrypt_command = new DecryptCommand;
}

EncryptedFile::Command* EncryptedFile::Open(const User& user)
{
	if (check_permission_write(user.getID())) {
		return open_command;
	}
	else {
		return error_massage_command->set_message("You have no rights");
	}
}

EncryptedFile::Command* EncryptedFile::Decrypt(const User& user)
{
	if (_owner->getID() == user.getID()) {
		return decrypt_command;
	}
	else {
		return error_massage_command->set_message("Only owner can decrypt that file");
	}
}

EncryptedFile::Command* EncryptedFile::Read(const User& user)
{
	if (check_permission_run(user.getID()) || check_permission_write(user.getID())) {
		for (size_t i = 0; i < _main.size(); ++i)
		{
			std::cout << _main[i].show(data);
		}
		_getch();
		return null_command;
	}
	else if (check_permission_read(user.getID())) {
		for (size_t i = 0; i < _symkey.size(); ++i)
		{
			std::cout << _symkey[i].show(data);
		}
		_getch();
		return null_command;
	}
	else
	{
		return error_massage_command->set_message("You have no rights");
	}
}

void EncryptedFile::File_Input(std::ifstream& fin)
{
	std::istream& in = fin;
	SystemObject::File_Input(fin);
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

std::pair<std::vector<Stream>&, std::vector<Stream>&> EncryptedFile::load_data()
{
	for (size_t i = 0; i < _main.size(); ++i)
	{
		_main[i].load(data);
	}
	for (size_t i = 0; i < _symkey.size(); ++i)
	{
		_symkey[i].load(data);
	}
	return { _main,_symkey };
}

void EncryptedFile::save_data(const std::string new_data)
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
		Stream t(get_free_block(), new_data.substr(symbols_read, Stream::block_size));
		_main.push_back(t);
		_symkey.push_back(Stream(get_free_block(), t.encrypt()));
		symbols_read += Stream::block_size;
	}

	save_data();
}

void EncryptedFile::save_data()
{
	auto comarator = [](const Stream& left, const Stream& right) -> bool
	{
		return left.get_offset() > right.get_offset();
	};
	std::priority_queue<Stream, std::vector<Stream>, std::function<bool(const Stream&, const Stream&)>> pq(comarator);
	for (size_t i = 0; i < _main.size(); ++i)
	{
		pq.push(_main[i]);
	}
	for (size_t i = 0; i < _symkey.size(); ++i)
	{
		pq.push(_symkey[i]);
	}
	for (size_t i = 0; i < pq.size(); ++i)
	{
		Stream t_stream = pq.top();
		t_stream.save(data);
		pq.pop();
	}
}

void EncryptedFile::delete_data()
{
	CommonFile::delete_data();
	for (size_t i = 0; i < _symkey.size(); ++i)
	{
		free_bloks.push(_symkey[i].get_offset());
	}
}

SystemObject* EncryptedFile::clone(SystemObject* new_parent, User* new_owner)
{
	auto date_and_time = get_current_date_and_time();
	EncryptedFile* copy_file = new EncryptedFile(new_parent, new_owner, date_and_time.first, date_and_time.second, _name);
	load_data();
	for (size_t i = 0; i < _main.size(); ++i)
	{
		copy_file->_main.push_back(Stream(get_free_block(), _main[i].get_buffer()));
	}
	for (size_t i = 0; i < _symkey.size(); ++i)
	{
		copy_file->_symkey.push_back(Stream(get_free_block(), _symkey[i].get_buffer()));
	}

	save_data();
	copy_file->save_data();
	return copy_file;
}

void EncryptedFile::show_self(size_t offset)
{
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	auto coords = GetConsoleCursorPosition(hConsole);
	gotoxy(hConsole, offset, coords.Y);
	std::cout << "(" << _type << ") " << _name << std::endl;
}
