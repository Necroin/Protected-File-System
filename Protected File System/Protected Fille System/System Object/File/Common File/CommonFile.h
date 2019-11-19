#pragma once
#ifndef _COMMONFILE_H_
#define _COMMONFILE_H_
#include "../../SystemObject.h"
#include "../../../Stream/Stream.h"
#include <queue>

class CommonFile : public SystemObject
{
public:
	class OpenCommand;
	class CopyCommand;
	class DeleteCommand;
	class EncryptCommand;
protected:
	std::vector<Stream> _main;
	User* _owner;
	inline static std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> free_bloks;
private:
	typedef Command* (CommonFile::* fptr)(const User& user);

	fptr actions[7] = {
		&CommonFile::Cancel,
		&CommonFile::Open,
		&CommonFile::Rename,
		&CommonFile::Copy,
		&CommonFile::Move,
		&CommonFile::Delete,
		&CommonFile::Encrypt
	};


	inline static const std::vector<std::string> actions_list = {
		"0.Cancel",
		"1.Open",
		"2.Rename",
		"3.Copy",
		"4.Move",
		"5.Delete",
		"6.Encrypt"
	};

	Command* Open(const User& user);
	Command* Copy(const User& user);
	Command* Move(const User& user);
	Command* Delete(const User& user);
	Command* Encrypt(const User& user);
public:
	CommonFile(User* owner, Date date, Time time, std::string path, std::string name = "New File");

	virtual ~CommonFile();
	static void Destroy_all_commands();

	virtual const std::vector<std::string>& get_actions_list() const override;
	virtual Command* get_command(size_t index, const User& user) override;
	virtual void Show();


};


class CommonFile::DeleteCommand : public Command
{
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};
#endif
