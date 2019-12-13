#include "SystemObject.h"
#include <list>
#include "Catalog/Catalog.h"
#include "../File System/FileSystem.h"
#include <conio.h>
#include "../Input/Input.h"


void SystemObject::delete_data()
{
	if (FileSystem::buffer_object == this) {
		reset_buffer();
	}
}

void SystemObject::Show()
{
	system("cls");
	std::list<std::string> path;
	SystemObject* p_obj = this;
	while (p_obj->_parent)
	{
		p_obj = p_obj->_parent;
		path.push_front(p_obj->_name);
	}
	std::cout << "Path : ";
	auto iter = path.begin();
	while (iter!=path.end())
	{
		std::cout << *iter++ << "\\";
	}
	std::cout << std::endl;
}

Pair<Date, Time> SystemObject::get_current_date_and_time()
{
	time_t t = time(NULL);
	tm* info = localtime(&t);
	Date date{ static_cast<size_t>(info->tm_mday), static_cast<size_t>(info->tm_mon + 1), static_cast<size_t>(1900 + info->tm_year) };
	Time time{ static_cast<size_t>(info->tm_hour),static_cast<size_t>(info->tm_min) };
	return { date, time };
}

SystemObject::Command* SystemObject::Cancel(const User& user)
{
	return null_command;
}

SystemObject::Command* SystemObject::Rename(const User& user)
{
	if (check_owner_or_admin(user.getID())) {
		return rename_command;
	}
	else {
		return error_massage_command->set_message("You have no rights");
	}
}

SystemObject::Command* SystemObject::Show_info(const User& user)
{

	if (check_permission_read(user.getID())) {
		std::cout << "Type of file : " << get_object_type() << std::endl;
		std::cout << _date << std::endl << _time << std::endl;
		_getch();
		return null_command;
	}
	else {
		return error_massage_command->set_message("You have no rights");
	}
}

SystemObject::Command* SystemObject::Show_my_permissions(const User& user)
{
	AccessSpecifiers access{false,false,false};
	if (check_owner_or_admin(user.getID())) {
		access = { true,true,true };
	}
	else {
		for (size_t i = 0; i < _users_access.size(); ++i)
		{
			auto user_access = _users_access[i];
			if (user_access.first == user.getID()) {
				access = user_access.second;
				break;
			}
		}
	}
	std::cout << "Your permissions :" << std::endl;
	std::cout << "Read  : " << (access._read ? "true" : "false") <<std::endl;
	std::cout << "Write : " << (access._write ? "true" : "false") << std::endl;
	std::cout << "Run   : " << (access._run ? "true" : "false") << std::endl;
	_getch();
	return null_command;
}

SystemObject::Command* SystemObject::Show_owner(const User& user)
{
	std::cout << _owner->get_name();
	_getch();
	return null_command;
}

SystemObject::Command* SystemObject::Change_permissions(const User& user)
{
	if (user.getID() == _owner->getID()) {
		std::cout << "Edit existing ones or add new ones?" << std::endl;
		std::cout << "0.Cancel" << std::endl;
		std::cout << "1.Edit existing ones" << std::endl;
		std::cout << "2.Add new" << std::endl;
		std::cout << "3.Delete existing one" << std::endl;
		int choise;
		get_object(std::cin, choise, [](int number) -> int {
			if (number < 0 || number > 3) {
				return 1;
			}
			return 0;
			});

		switch (choise)
		{
		case 1: return change_permissions_command; break;
		case 2: return add_permissions_command; break;
		case 3: return delete_permissions_command; break;
		default:
			break;
		}
	}
	else {
		return error_massage_command->set_message("Only owner can change the permissions");
	}
	return null_command;
}

void SystemObject::reset_buffer()
{
	FileSystem::buffer_command = nullptr;
	FileSystem::buffer_object = nullptr;
	FileSystem::buffer_command_type = "None";
}

SystemObject::Command* SystemObject::Move(const User& user)
{
	if (check_owner_or_admin(user.getID())) {
		FileSystem::buffer_command = move_command;
		FileSystem::buffer_object = this;
		FileSystem::buffer_command_type = "Move";
		return null_command;
	}
	else {
		return error_massage_command->set_message("You have no rights");
	}
}

SystemObject::Command* SystemObject::Copy(const User& user)
{
	if (check_permission_read(user.getID())) {
		FileSystem::buffer_command = copy_command;
		FileSystem::buffer_object = this;
		FileSystem::buffer_command_type = "Copy";
		return null_command;
	}
	else {
		return error_massage_command->set_message("You have no rights");
	}
}

SystemObject::Command* SystemObject::Delete(const User& user)
{
	if (check_owner_or_admin(user.getID())) {
		return delete_command;
	}
	else {
		return error_massage_command->set_message("You have no rights");
	}
}

SystemObject::SystemObject(SystemObject* catalog, User* owner, Date date, Time time, std::string name) :
	_date(date),
	_time(time),
	_parent(catalog),
	_name(name),
	_owner(owner)
{}

SystemObject::SystemObject(SystemObject * catalog) : _parent(catalog)
{}

SystemObject::SystemObject() {}

SystemObject::~SystemObject() {}

void SystemObject::Destroy_all_commands()
{
	delete null_command;
	delete rename_command;
	delete move_command;
	delete copy_command;
	delete delete_command;
	delete change_permissions_command;
	delete add_permissions_command;
	delete delete_permissions_command;
	delete error_massage_command;
}

void SystemObject::Init_all_commands()
{
	null_command = new Command;
	rename_command = new RenameCommand;
	delete_command = new DeleteCommand;
	copy_command = new CopyCommand;
	move_command = new MoveCommand;
	change_permissions_command = new ChangePermissionsCommand;
	add_permissions_command = new AddPermissionsCommand;
	delete_permissions_command = new DeletePermissionsCommand;
	error_massage_command = new ErrorMessageCommand;
}

SystemObject& SystemObject::set_time(const Time & time)
{
	_time = time;
	return *this;
}

SystemObject& SystemObject::set_date(const Date& date)
{
	_date = date;
	return *this;
}

SystemObject& SystemObject::set_name(const std::string& name)
{
	_name = name;
	return *this;
}

const Time& SystemObject::get_time() const
{
	return _time;
}

const Date& SystemObject::get_date() const
{
	return _date;
}

const std::string& SystemObject::get_name() const
{
	return _name;
}

const std::string SystemObject::get_path() const
{
	std::string path("that is path");
	return path;
}

bool SystemObject::have_parent()
{
	return _parent ? true : false;
}

SystemObject* SystemObject::get_parent()
{
	return _parent;
}

std::ifstream& operator>>(std::ifstream& fin, SystemObject& object)
{
	object.File_Input(fin);
	return fin;
}

void SystemObject::File_Input(std::ifstream& fin)
{
	fin >> _date >> _time >> _name;
	// Считывание ID владельца
	ID owner_id;
	fin >> owner_id;
	// Поиск владельца по ID
	_owner = UsersTable::find(owner_id);
	if (_owner) {
		_owner->add_object(this);
	}
	size_t Id_count;
	fin >> Id_count;
	for (size_t i = 0; i < Id_count; ++i)
	{
		ID id;
		AccessSpecifiers access;
		fin >> id;
		fin >> access;
		_users_access.push_back(std::pair<ID, AccessSpecifiers>{id, access});
	}
}

std::ofstream& operator<<(std::ofstream& fout, const SystemObject& object)
{
	object.File_Output(fout);
	return fout;
}

void SystemObject::File_Output(std::ofstream& fout) const
{
	// Вывод даты, времени и имени
	fout << _date << " ";
	fout << _time << " ";
	static_cast<std::ostream&>(fout) << _name;
	fout << std::endl;
	// Вывод ID владельца
	fout << _owner->getID() << std::endl;
	// Определение и вывод количества пользователей с доступом
	size_t Id_count = _users_access.size();
	fout << Id_count << std::endl;
	// Вывод всех пользователей ( ID и их права)
	for (size_t i = 0; i < Id_count; ++i)
	{
		auto pair = _users_access[i];
		fout << pair.first << " ";
		fout << pair.second << std::endl;
	}
}

bool SystemObject::check_permission_read(ID id)
{
	if (check_owner_or_admin(id)) { return true; }
	for (size_t i = 0; i < _users_access.size(); ++i)
	{
		if (_users_access[i].first == id) return _users_access[i].second._read;
	}
	return false;
}

bool SystemObject::check_permission_write(ID id)
{
	if (check_owner_or_admin(id)) { return true; }
	for (size_t i = 0; i < _users_access.size(); ++i)
	{
		if (_users_access[i].first == id) return _users_access[i].second._write;
	}
	return false;
}

bool SystemObject::check_permission_run(ID id)
{
	if (check_owner_or_admin(id)) { return true; }
	for (size_t i = 0; i < _users_access.size(); ++i)
	{
		if (_users_access[i].first == id) return _users_access[i].second._run;
	}
	return false;
}

bool SystemObject::check_owner_or_admin(ID id)
{
	return (id == _owner->getID() || id == 1) ? true : false;
}
