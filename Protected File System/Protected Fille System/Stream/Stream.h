#pragma once
#ifndef _STREAM_H_
#define _STREAM_H_
#include <string>
#include <fstream>

class Stream
{
private:
	size_t _offset;
	std::string _buffer;
public:
	inline static const size_t block_size = 10;
	Stream(size_t offset);
	Stream(size_t offset, std::string buffer);
	size_t get_offset() const;
	const std::string& get_buffer() const;
	std::string get_buffer();
	bool empty();
	void add_to_buffer(std::string additional);

	void save(std::fstream& fout);
	void load(std::fstream& fin);
	std::string show(std::fstream& fin);
};
#endif
