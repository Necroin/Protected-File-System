#include "CommonFile.h"

CommonFile::CommonFile(User* owner, Date date, Time time, std::string path, std::string name) : SystemObject(date, time, path, name), _owner(owner)
{

}

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

void CommonFile::File_Input(std::ifstream& fin)
{
	std::istream& in = fin;
	SystemObject::File_Input(fin);
}

void CommonFile::File_Output(std::ofstream& fout) const
{
	std::ostream& out = fout;
	out << "CommonFile" << " ";
	SystemObject::File_Output(fout);
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
