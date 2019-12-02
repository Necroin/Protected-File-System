#pragma once
#ifndef _STREAM_H_
#define _STREAM_H_
#include <string>

class Stream
{
private:
	size_t _offset;
	inline static const size_t block_size = 100;
	std::string _buffer;
public:
	Stream(size_t offset);
	size_t get_offset() const;
	const std::string& get_buffer() const;
	std::string get_buffer();
	bool empty();
};
#endif
