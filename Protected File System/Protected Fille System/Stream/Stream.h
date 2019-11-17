#pragma once
#ifndef _STREAM_H_
#define _STREAM_H_
#include <string>

class Stream
{
private:
	size_t _offset;
	inline static const size_t block_size = 100;
public:
	Stream(std::string name, size_t offset);
};
#endif
