#include "Protected Fille System/File System/FileSystem.h"



int main(int argc, char* args) {
	FileSystem fs("","\\");
	fs.start();
	fs.log_on();
	fs.run();
	fs.stop();
	system("pause");
	return 0;
}