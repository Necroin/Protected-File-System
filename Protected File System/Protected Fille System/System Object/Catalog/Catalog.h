#pragma once
#ifndef _CATALOG_H_
#define _CATALOG_H_
#include "../File/Encrypted File/EncryptedFile.h"


class Catalog : public SystemObject
{
public:
	class OpenCommand;
	class CopyCommand;
	class DeleteCommand;
	class CreateFileCommand;
	class CreateCatalogCommand;
	class BackUpCommand;
	class LogOutCommand;
private:
	inline static OpenCommand* open_command;
	inline static CopyCommand* copy_command;
	inline static DeleteCommand* delete_command;
	inline static CreateFileCommand* create_file_command;
	inline static CreateCatalogCommand* create_catalog_command;
	inline static BackUpCommand* back_up_command;
	inline static LogOutCommand* log_out_command;


	std::vector<SystemObject*> _objects;
	Catalog* _parent = nullptr;
	typedef Command* (Catalog::* fptr)(const User& user);

	fptr actions[6] = {
		&Catalog::Cancel,
		&Catalog::Open,
		&Catalog::Rename,
		&Catalog::Copy,
		&Catalog::Move,
		&Catalog::Delete
	};


	inline static const std::vector<std::string> actions_list = {
		"0.Cancel",
		"1.Open",
		"2.Rename",
		"3.Copy",
		"4.Move",
		"5.Delete"
	};

	inline static const std::vector<std::string> actions_list_in_ = {
		"0.Cancel",
		"1.Create File",
		"2.Create Catalog",
		"3.Log out",
		"4.Back Up"
	};

	fptr actions_in_[5] = {
		&Catalog::Cancel,
		&Catalog::Create_File,
		&Catalog::Create_Catalog,
		&Catalog::LogOut,
		&Catalog::BackUp
	};

	Command* Open(const User& user);
	Command* Copy(const User& user);
	Command* Move(const User& user);
	Command* Delete(const User& user);
	Command* Create_File(const User& user);
	Command* Create_Catalog(const User& user);
	Command* BackUp(const User& user);
	Command* LogOut(const User& user);
public:
	Catalog(Catalog* parent, Date date, Time time, std::string path, std::string name = "New Folder");

	~Catalog();
	static void Destroy_all_commands();

	virtual const std::vector<std::string>& get_actions_list() const override;
	virtual Command* get_command(size_t index, const User& user) override;

	void Show_in_actions();
	Command* Choose_in_actions(User& user);

	virtual void Show() override;
	size_t Wait();
	bool have_parent();
	Catalog* get_parent();

	SystemObject* operator[](size_t index) { return _objects[index]; }


	SystemObject* find(std::string name);
	void erase(SystemObject*& other);
	void add_object(SystemObject* object);
};

class Catalog::OpenCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class Catalog::CopyCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class Catalog::DeleteCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class Catalog::CreateFileCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class Catalog::CreateCatalogCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class Catalog::BackUpCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class Catalog::LogOutCommand : public Command
{
public: virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};
#endif