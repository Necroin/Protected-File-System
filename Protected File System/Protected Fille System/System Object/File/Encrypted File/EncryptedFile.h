#pragma once
#ifndef _ENCRYPTEDFILE_H_
#define _ENCRYPTEDFILE_H_
#include "../Common File/CommonFile.h"

class EncryptedFile : public CommonFile
{
protected:
	std::vector<Stream> _symkey;
private:

	typedef Command* (EncryptedFile::* fptr)(const User& user);

	fptr actions[7] = {
		&EncryptedFile::Cancel,
		&EncryptedFile::Open,
		&EncryptedFile::Rename,
		&EncryptedFile::Copy,
		&EncryptedFile::Move,
		&EncryptedFile::Delete,
		&EncryptedFile::Decrypt
	};

	inline static const std::vector<std::string> actions_list = {
		"0.Cancel",
		"1.Open",
		"2.Rename",
		"3.Copy",
		"4.Move",
		"5.Delete",
		"6.Decrypt"
	};

	Command* Open(const User& user);
	Command* Copy(const User& user);
	Command* Move(const User& user);
	Command* Delete(const User& user);
	Command* Decrypt(const User& user);
public:
	EncryptedFile(User* owner, Date date, Time time, std::string path, std::string name = "New Encr_File");

	virtual const std::vector<std::string>& get_actions_list() const override;
	virtual Command* get_command(size_t index, const User& user) override;
	virtual void Show();


};
#endif
