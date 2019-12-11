#pragma once
#ifndef _ENCRYPTEDFILE_H_
#define _ENCRYPTEDFILE_H_
#include "../Common File/CommonFile.h"

class EncryptedFile : public CommonFile
{
public:
	class DecryptCommand;
	class OpenCommand;
protected:
	std::vector<Stream> _symkey;
private:
	friend class CommonFile::EncryptCommand;

	inline static OpenCommand* open_command;
	inline static DecryptCommand* decrypt_command;

	inline static const std::string _type = "Encrypted File";

	typedef Command* (EncryptedFile::* fptr)(const User& user);

	fptr actions[12] = {
		&EncryptedFile::Cancel,
		&EncryptedFile::Open,
		&EncryptedFile::Read,
		&EncryptedFile::Rename,
		&EncryptedFile::Copy,
		&EncryptedFile::Move,
		&EncryptedFile::Delete,
		&EncryptedFile::Decrypt,
		&EncryptedFile::Show_owner,
		&EncryptedFile::Show_info,
		&EncryptedFile::Show_my_permissions,
		&EncryptedFile::Change_permissions
	};

	inline static const std::vector<std::string> actions_list = {
		"0.  Cancel",
		"1.  Open for write",
		"2.  Read",
		"3.  Rename",
		"4.  Copy",
		"5.  Move",
		"6.  Delete",
		"7.  Decrypt",
		"8.  Show owner",
		"9.  Show info",
		"10. Show my permissions",
		"11. Change permissions"
	};

	Command* Open(const User& user);
	Command* Decrypt(const User& user);
	Command* Read(const User& user);


	virtual void File_Input(std::ifstream& fin) override;
	virtual void File_Output(std::ofstream& fout) const override;
public:
	EncryptedFile(SystemObject* catalog, User* owner, Date date, Time time, std::string name);
	EncryptedFile(SystemObject* catalog);
	~EncryptedFile();
	static void Destroy_all_commands();
	static void Init_all_commands();

	virtual const std::vector<std::string>& get_actions_list() const override;
	virtual Command* get_command(size_t index, const User& user) override;
	virtual void Show();
	virtual const std::string& get_object_type() const override;



	std::pair<std::vector<Stream>&, std::vector<Stream>&> load_data();
	void save_data(const std::string new_data);
	void save_data();

	virtual void delete_data() override;
	virtual SystemObject* clone(SystemObject* new_parent, User* new_owner) override;

	virtual void show_self(size_t offset) override;
};


class EncryptedFile::OpenCommand : public Command
{
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};

class EncryptedFile::DecryptCommand : public Command
{
	virtual bool execute(SystemObject*& catalog, SystemObject*& object, User*& user) override;
};
#endif
