#include "Catalog.h"
#include "../../Input/Input.h"
#include "../../WinhFunc/WinhFunc.h"
#include <conio.h>
#include "../../User/Table/UsersTable.h"


Catalog::Catalog(Catalog* parent, User* user, Date date, Time time, std::string name) :
	SystemObject(parent, user, date, time, name)
{}

Catalog::Catalog(Catalog* parent) :
	SystemObject(parent)
{}

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
	delete create_file_command;
	delete create_catalog_command;
	delete back_up_command;
	delete put_in_command;
}

void Catalog::Init_all_commands()
{
	open_command = new OpenCommand;
	create_file_command = new CreateFileCommand;
	create_catalog_command = new CreateCatalogCommand;
	back_up_command = new BackUpCommand;
	log_out_command = new LogOutCommand;
	put_in_command = new PutInCommand;
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
	get_object(std::cin, index,
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

const std::string& Catalog::get_object_type() const
{
	return _type;
}

void Catalog::Show()
{
	SystemObject::Show();
	std::cout << "Catalog : " << _name << std::endl << std::endl;

	std::cout << "0.Actions in _current_" << std::endl;
	std::cout << std::endl;

	for (size_t i = 0; i < _objects.size(); i++)
	{
		std::cout << i + 1 << "." << "(" << _objects[i]->get_object_type() << ") " << _objects[i]->get_name() << std::endl;
	}
	std::cout << std::endl;
}

size_t Catalog::Wait()
{
	int choise, lower_limit = 0;
	int size = _objects.size();
	std::cout << "Choose system object to interact with";
	std::cout << " -->";
	get_object(std::cin,choise,
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

void Catalog::delete_data()
{
	SystemObject::delete_data();
	for (size_t i = 0; i < _objects.size(); ++i)
	{
		_objects[i]->delete_data();
		delete _objects[i];
	}
}

SystemObject* Catalog::clone(SystemObject* new_parent, User* new_owner)
{
	auto data_and_time = get_current_date_and_time();
	Catalog* copy_catalog = new Catalog(static_cast<Catalog*>(new_parent), new_owner, data_and_time.first, data_and_time.second, _name);
	for (size_t i = 0; i < _objects.size(); ++i)
	{
		copy_catalog->add_object(this->_objects[i]->clone(this, new_owner));
	}
	return copy_catalog;
}

SystemObject* Catalog::find(std::string name)
{
	for (size_t i = 0; i < _objects.size(); ++i)
	{
		if (_objects[i]->get_name() == name) { return _objects[i]; }
	}
	return nullptr;
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

Catalog::Command* Catalog::Show_data(const User& user)
{
	size_t offset = 0;
	this->show_self(offset);
	_getch();
	return null_command;
}

Catalog::Command* Catalog::Put_in(const User& user)
{
	return put_in_command;
}

Catalog::Command* Catalog::Edit_users_table(const User& user)
{
	if (user.getID() == 1) {
		UsersTable::EditTable();
	}
	else {
		return error_massage_command->set_message("Only Admin can edit the users table");
	}
	return null_command;
}

void Catalog::File_Input(std::ifstream& fin)
{
	std::istream& in = fin;
	// Общеее считывание
	SystemObject::File_Input(fin);
	// Количество вложенных объектов
	size_t size;
	std::string object_type;
	fin >> size;
	for (size_t i = 0; i < size; i++)
	{
		SystemObject* object = nullptr;
		fin >> object_type;
		if (object_type == "Catalog") {
			object = new Catalog(this);

		}
		else if (object_type == "CommonFile") {
			object = new CommonFile(this);
		}
		else if (object_type == "EncryptedFile") {
		}

		if (object != nullptr) {
			fin >> *object;
		}
		else {
			throw std::exception("Descriptors corrupted");
		}
		_objects.push_back(object);
	}
}

void Catalog::File_Output(std::ofstream& fout) const
{
	std::ostream& out = fout;
	// Оаознаватель каталога
	out << "Catalog" << " ";
	// Стандартный вывод
	SystemObject::File_Output(fout);
	// Определение и вывод количества вложенных объектов
	size_t size = _objects.size();
	out << size << std::endl;
	// Вывод всех вложенных объектов
	for (size_t i = 0; i < size; ++i)
	{
		fout << (*_objects[i]);
	}
}

void Catalog::show_self(size_t offset)
{
	//static const char corner = 192;
	//static const char dash = 196;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	auto coords = GetConsoleCursorPosition(hConsole);
	gotoxy(hConsole, offset, coords.Y);
	std::cout /*<< (char)corner << (char)dash << (char)dash*/ << "(" << _type << ") " << _name << std::endl;
	for (size_t i = 0; i < _objects.size(); ++i)
	{
		_objects[i]->show_self(offset + 3);
	}

}
