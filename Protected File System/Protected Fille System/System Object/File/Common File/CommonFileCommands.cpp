#include "CommonFile.h"
#include "../../Catalog/Catalog.h"
#include "../Encrypted File/EncryptedFile.h"


bool CommonFile::OpenCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	std::vector<Stream>& main_streams = static_cast<CommonFile*>(object)->load_data();

	for (size_t i = 0; i < main_streams.size(); i++)
	{
		std::cout << main_streams[i].get_buffer();
	}

	std::cout << std::endl << "Enter new data --> ";
	std::string new_data;
	std::getline(std::cin, new_data);


	static_cast<CommonFile*>(object)->save_data(new_data);

	return true;
}


bool CommonFile::EncryptCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	CommonFile& that_file = static_cast<CommonFile&>(*object);
	EncryptedFile* new_file = new EncryptedFile(that_file._parent, that_file._owner, that_file._date, that_file._time, that_file._name);
	new_file->_users_access = that_file._users_access; //?
	new_file->_main = that_file._main;
	auto& symkey = new_file->_symkey;
	auto& main = new_file->_main;
	for (size_t i = 0; i < main.size(); ++i)
	{
		symkey.push_back(Stream(get_free_block(),main[i].encrypt()));
	}

	object = new_file;
	delete &that_file;
	new_file->save_data();
	return true;
}