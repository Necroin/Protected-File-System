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
	class EncryptCommand;
protected:
	std::vector<Stream> _main;
	inline static std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> free_bloks;
	size_t get_free_block();
	inline static std::fstream data;
private:
	inline static OpenCommand* open_command;
	inline static EncryptCommand* encrypt_command;

	inline static const std::string _type = "Common File";

	typedef Command* (CommonFile::* fptr)(const User& user);

	fptr actions[12] = {
		&CommonFile::Cancel,
		&CommonFile::Open,
		&CommonFile::Read,
		&CommonFile::Rename,
		&CommonFile::Copy,
		&CommonFile::Move,
		&CommonFile::Delete,
		&CommonFile::Encrypt,
		&CommonFile::Show_owner,
		&CommonFile::Show_info,
		&CommonFile::Show_my_permissions,
		&CommonFile::Change_permissions
	};


	inline static const std::vector<std::string> actions_list = {
		"0.  Cancel",
		"1.  Open for write",
		"2.  Read",
		"3.  Rename",
		"4.  Copy",
		"5.  Move",
		"6.  Delete",
		"7.  Encrypt",
		"8.  Show owner",
		"9.  Show info",
		"10. Show my permissions",
		"11. Change permissions"
	};

	Command* Open(const User& user);
	Command* Encrypt(const User& user);
	Command* Read(const User& user);

	virtual void File_Input(std::ifstream& fin) override;
	virtual void File_Output(std::ofstream& fout) const override;
public:
	CommonFile(SystemObject* catalog,User* owner, Date date, Time time, std::string name);
	CommonFile(SystemObject* catalog);

	virtual ~CommonFile();
	static void Destroy_all_commands();
	static void Init_all_commands();

	virtual const std::vector<std::string>& get_actions_list() const override;
	virtual Command* get_command(size_t index, const User& user) override;
	virtual void Show();
	virtual const std::string& get_object_type() const override;

	static bool open_data_file(std::string path);
	static bool close_data_file();
	static bool load_free_blocks(std::string path);
	static bool save_free_blocks(std::string path);

	std::vector<Stream>& load_data();
	void save_data(const std::string new_data);
	void save_data();

	virtual void delete_data() override;
	virtual SystemObject* clone(SystemObject* new_parent, User* new_owner) override;

	virtual void show_self(size_t offset) override;
};

class CommonFile::OpenCommand : public Command
{
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class CommonFile::EncryptCommand : public Command
{
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};
#endif
