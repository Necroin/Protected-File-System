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
	class CopyCommand;
	class DeleteCommand;
	class ChangePermissionsCommand;
	class AddPermissionsCommand;
	class DeletePermissionsCommand;
private:
	friend class MoveCommand;
	friend class CopyCommand;
	friend class RenameCommand;
	friend class DeleteCommand;
	friend class ChangePermissionsCommand;
	friend class AddPermissionsCommand;
	friend class DeletePermissionsCommand;
protected:
	Time _time;
	Date _date;
	std::string _name;
	SystemObject* _parent = nullptr;

	User* _owner = nullptr;

	std::vector<std::pair<ID, AccessSpecifiers>> _users_access;

	inline static Command* null_command;
	inline static RenameCommand* rename_command;
	inline static MoveCommand* move_command;
	inline static CopyCommand* copy_command;
	inline static DeleteCommand* delete_command;
	inline static ChangePermissionsCommand* change_permissions_command;
	inline static AddPermissionsCommand* add_permissions_command;
	inline static DeletePermissionsCommand* delete_permissions_command;
	inline static ErrorMessageCommand* error_massage_command;


	Command* Cancel(const User& user);
	Command* Move(const User& user);
	Command* Copy(const User& user);
	Command* Delete(const User& user);
	Command* Rename(const User& user);
	Command* Show_info(const User& user);
	Command* Change_permissions(const User& user);


	static void reset_buffer();

	virtual void File_Input(std::ifstream& fin);
	virtual void File_Output(std::ofstream& fout) const;

	bool check_permission_read(ID id);
	bool check_permission_write(ID id);
	bool check_permission_run(ID id);
	bool check_owner_or_admin(ID id);
public:
	SystemObject(SystemObject* catalog, User* owner, Date date, Time time, std::string name);
	SystemObject(SystemObject* catalog);
	SystemObject();
	virtual ~SystemObject();
	static void Destroy_all_commands();
	static void Init_all_commands();

	// Setters
	SystemObject& set_time (const Time&   time);
	SystemObject& set_date (const Date&   date);
	SystemObject& set_name (const std::string& name);

	//Getters
	const Time&   get_time () const;
	const Date&   get_date () const;
	const std::string& get_name () const;
	const std::string get_path () const;
	virtual const std::string& get_object_type() const = 0;

	virtual const std::vector<std::string>& get_actions_list() const = 0;
	virtual Command* get_command(size_t index, const User& user) = 0;

	virtual void delete_data();
	virtual SystemObject* clone(SystemObject* new_parent, User* new_owner) = 0;

	virtual void Show();

	bool have_parent();
	SystemObject* get_parent();

	static Pair<Date, Time> get_current_date_and_time();

	friend std::ifstream& operator >> (std::ifstream& fin, SystemObject& object);
	friend std::ofstream& operator << (std::ofstream& fout, const SystemObject& object);

	virtual void show_self(size_t offset) = 0;
};

class SystemObject::Command {
public:
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user);
};

class SystemObject::ErrorMessageCommand : public Command
{
private:
	std::string _message;
public:
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
	ErrorMessageCommand* set_message(std::string message) { _message = message; return this; }
};

class SystemObject::RenameCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::MoveCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::CopyCommand : public Command
{
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::DeleteCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::ChangePermissionsCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::AddPermissionsCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class SystemObject::DeletePermissionsCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};
#endif



