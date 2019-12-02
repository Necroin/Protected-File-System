#include "SystemObject.h"
#include <list>
#include "Catalog/Catalog.h"


void SystemObject::Show()
{
	system("cls");
	std::string path("there must be a path");
	std::cout << "Path : " << path << std::endl;
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
	return rename_command;
}

SystemObject::SystemObject(SystemObject* catalog, Date date, Time time, std::string name) :
	_date(date),
	_time(time),
	_parent(catalog),
	_name(name)
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
}

void SystemObject::Init_all_commands()
{
	null_command = new Command;
	rename_command = new RenameCommand;
	move_command = new MoveCommand;
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

std::ifstream& operator>>(std::ifstream& fin, SystemObject& object)
{
	object.File_Input(fin);
	return fin;
}

void SystemObject::File_Input(std::ifstream& fin)
{
	fin >> _date >> _time >> _name;
	size_t Id_count;
	fin >> Id_count;
	for (size_t i = 0; i < Id_count; ++i)
	{
		ID id;
		AccessSpecifiers access;
		fin >> id;
		fin >> access;
		users_access.push_back(std::pair<ID, AccessSpecifiers>{id, access});
	}
}

std::ofstream& operator<<(std::ofstream& fout, const SystemObject& object)
{
	object.File_Output(fout);
	return fout;
}

void SystemObject::File_Output(std::ofstream& fout) const
{
	// ¬ывод даты, времени и имени
	fout << _date << " ";
	fout << _time << " ";
	static_cast<std::ostream&>(fout) << _name;
	fout << std::endl;
	// ќпределение и вывод количества пользователей с доступом
	size_t Id_count = users_access.size();
	fout << Id_count << std::endl;
	// ¬ывод всех пользователей ( ID и их права)
	for (size_t i = 0; i < Id_count; ++i)
	{
		auto pair = users_access[i];
		fout << pair.first << " ";
		fout << pair.second << std::endl;
	}
}
