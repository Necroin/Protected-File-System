#include "CommonFile.h"
#include "../../Catalog/Catalog.h"


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
	return true;
}