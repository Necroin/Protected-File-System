#pragma once
#ifndef _SYSTEMOBJECT_H_
#define _SYSTEMOBJECT_H_
#include <ctime>
#include "../Support Structures/Date.h"
#include "../Support Structures/Time.h"
#include "../Support Structures/Pair.h"
#include "../User/User.h"

class SystemObject
{
public:
	class Command;
	class ErrorMessageCommand;
	class RenameCommand;
	class MoveCommand;
protected:
	Time _time;
	Date _date;
	std::string _name;
	std::string _path;

	std::vector<std::pair<ID, AccessSpecifiers>> users_access;

	inline static Command *null_command;
	inline static RenameCommand* rename_command;
	inline static MoveCommand* move_command;

	Command* Cancel(const User& user);
	Command* Rename(const User& user);


	virtual void File_Input(std::ifstream& fin);
	virtual void File_Output(std::ofstream& fout) const;
public:
	SystemObject(Date date, Time time, std::string path, std::string name);
	SystemObject();
	SystemObject(std::string path);
	virtual ~SystemObject();
	static void Destroy_all_commands();
	static void Init_all_commands();

	// Setters
	SystemObject& set_time (const Time&   time);
	SystemObject& set_date (const Date&   date);
	SystemObject& set_name (const std::string& name);
	SystemObject& set_path (const std::string& path);

	//Getters
	const Time&   get_time () const;
	const Date&   get_date () const;
	const std::string& get_name () const;
	const std::string& get_path () const;
	virtual const std::string& get_object_type() const = 0;

	virtual const std::vector<std::string>& get_actions_list() const = 0;
	virtual Command* get_command(size_t index, const User& user) = 0;


	virtual void Show();

	static Pair<Date, Time> get_current_date_and_time();

	friend std::ifstream& operator >> (std::ifstream& fin, SystemObject& object);
	friend std::ofstream& operator << (std::ofstream& fout, const SystemObject& object);
};

class SystemObject::Command {
public:
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user);
};

class SystemObject::ErrorMessageCommand : Command
{
private:
	std::string _message;
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
	void set_message(std::string message) { _message = message; }
};

class SystemObject::RenameCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::MoveCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};
#endif



