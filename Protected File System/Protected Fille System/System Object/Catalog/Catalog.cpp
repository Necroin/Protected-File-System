#include "Catalog.h"
#include "../../Input/Input.h"

Catalog::Catalog(Catalog* parent, Date date, Time time, std::string path, std::string name) :
	SystemObject(date, time, path, name),
	_parent(parent)
{}

Catalog::Catalog() {}

Catalog::~Catalog()
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		delete _objects[i];
	}
}

void Catalog::Destroy_all_commands()
{
	delete open_command;
	delete copy_command;
	delete delete_command;
	delete create_file_command;
	delete create_catalog_command;
	delete back_up_command;
}

void Catalog::Init_all_commands()
{
	open_command = new OpenCommand;
	copy_command = new CopyCommand;
	delete_command = new DeleteCommand;
	create_file_command = new CreateFileCommand;
	create_catalog_command = new CreateCatalogCommand;
	back_up_command = new BackUpCommand;
	log_out_command = new LogOutCommand;
}

void Catalog::Show_in_actions()
{
	size_t count = _parent ? actions_list_in_.size() : actions_list_in_.size() - 1;
	for (size_t i = 0; i < count; ++i)
	{
		std::cout << actions_list_in_[i] << std::endl;
	}
	std::cout << std::endl;
}

Catalog::Command* Catalog::Choose_in_actions(User& user)
{
	int index, size = _parent ? actions_list_in_.size() : actions_list_in_.size() - 1;
	std::cout << "Choose -->";
	get_number(std::cin, index,
		[size](int number) {
			if (number < 0 || number >= size) {
				std::cout << "Your input must be >= 0 and <= " << size-1 << std::endl;
				return 1;
			}
			return 0;
		});
	return (this->*actions_in_[index])(user);
}

const std::vector<std::string>& Catalog::get_actions_list() const { return actions_list; }

Catalog::Command* Catalog::get_command(size_t index, const User& user)
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

void Catalog::Show()
{
	SystemObject::Show();
	std::cout << "Catalog : " << _name << std::endl << std::endl;

	std::cout << "0.Actions in _current_" << std::endl;
	std::cout << std::endl;

	for (size_t i = 0; i < _objects.size(); i++)
	{
		std::cout << i + 1 << "." << _objects[i]->get_name() << std::endl;
	}
	std::cout << std::endl;
}

size_t Catalog::Wait()
{
	int choise, lower_limit = 0;
	int size = _objects.size();
	std::cout << "Choose system object to interact with";
	std::cout << " -->";
	get_number(std::cin,choise,
		[size,lower_limit](int number) -> int {
			if (number < lower_limit || number > size) {
				std::cout << "Your input must be >= " << lower_limit << " and <="  << size << std::endl;
				return 1;
			}
			return 0;
		}
	);
	return static_cast<size_t>(choise);
}

bool Catalog::have_parent()
{
	return _parent ? true : false;
}

Catalog* Catalog::get_parent()
{
	return _parent;
}

SystemObject* Catalog::find(std::string name)
{
	for (size_t i = 0; i < _objects.size(); ++i)
	{
		if (_objects[i]->get_name() == name) { return _objects[i]; }
	}
	return nullptr;
}

std::vector<SystemObject*> Catalog::find_all(std::string name)
{
	return std::vector<SystemObject*>();
}

void Catalog::erase(SystemObject*& other)
{
	std::vector<SystemObject*>::iterator erase_it = _objects.begin();
	size_t i = 0;
	while (_objects[i] != other)
	{
		++i;
		++erase_it;
	}
	_objects.erase(erase_it);
}


void Catalog::add_object(SystemObject* object)
{
	_objects.push_back(object);
}



Catalog::Command* Catalog::Open(const User& user)
{
	return open_command;
}

Catalog::Command* Catalog::Copy(const User& user)
{
	return copy_command;
}

Catalog::Command* Catalog::Move(const User& user)
{
	return move_command;
}

Catalog::Command* Catalog::Delete(const User& user)
{
	return delete_command;
}

Catalog::Command* Catalog::Create_File(const User& user)
{
	return create_file_command;
}

Catalog::Command* Catalog::Create_Catalog(const User& user)
{
	return create_catalog_command;
}

Catalog::Command* Catalog::BackUp(const User& user)
{
	return back_up_command;
}

Catalog::Command* Catalog::LogOut(const User& user)
{
	return log_out_command;
}

void Catalog::File_Input(std::ifstream& fin)
{
	std::istream& in = fin;
	SystemObject::File_Input(fin);
}

void Catalog::File_Output(std::ofstream& fout) const
{
	std::ostream& out = fout;
	out << "Catalog" << " ";
	SystemObject::File_Output(fout);
	out << std::endl;
	size_t size = _objects.size();
	out << size << std::endl;
	for (size_t i = 0; i < size; ++i)
	{
		fout << (*_objects[i]) << std::endl;
	}
}
