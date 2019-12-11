#include "EncryptedFile.h"
#include "../../Catalog/Catalog.h"


bool EncryptedFile::DecryptCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	EncryptedFile& that_file = static_cast<EncryptedFile&>(*object);
	CommonFile* new_file = new CommonFile(that_file);

	object = new_file;
	delete& that_file;
	return true;
}

bool EncryptedFile::OpenCommand::execute(SystemObject*& catalog, SystemObject*& object, User*& user)
{
	auto all_streams = static_cast<EncryptedFile*>(object)->load_data();

	for (size_t i = 0; i < all_streams.first.size(); ++i)
	{
		std::cout << all_streams.first[i].get_buffer();
	}

	std::cout << std::endl << "Enter new data --> ";
	std::string new_data;
	std::getline(std::cin, new_data);


	static_cast<CommonFile*>(object)->save_data(new_data);

	return true;
	return true;
}