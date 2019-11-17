#include "SystemObject.h"


void SystemObject::Show()
{
	system("cls");
	std::cout << "Path : " << _path << std::endl;
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

SystemObject::SystemObject(Date date, Time time, std::string path, std::string name) :
	_date(date),
	_time(time),
	_path(path),
	_name(name)
{
	null_command   = new Command;
	rename_command = new RenameCommand;
	move_command   = new MoveCommand;
}

SystemObject::~SystemObject()
{
	delete null_command;
	delete rename_command;
	delete move_command;
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

SystemObject& SystemObject::set_path(const std::string& path)
{
	_path = path;
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

const std::string& SystemObject::get_path() const
{
	return _path;
}