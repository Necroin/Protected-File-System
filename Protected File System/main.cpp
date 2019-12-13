#include "Protected Fille System/File System/FileSystem.h"



int main(int argc, char* args) {
	FileSystem fs("D:/C++/Protected File System/Protected File System/Protected Fille System/Data/Paths.txt");
	fs.start();
	fs.run();
	fs.stop();
	system("pause");
	return 0;
}