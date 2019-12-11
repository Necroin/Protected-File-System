#include "Stream.h"

Stream::Stream(size_t offset) : _offset(offset)
{}

Stream::Stream(size_t offset, std::string buffer) : 
	_offset(offset),
	_buffer(buffer)
{}

size_t Stream::get_offset() const
{
	return _offset;
}

const std::string& Stream::get_buffer() const
{
	return _buffer;
}

std::string Stream::get_buffer()
{
	return _buffer;
}

bool Stream::empty()
{
	return _buffer.empty();
}

void Stream::add_to_buffer(std::string additional)
{
	if (_buffer.size() + additional.size() > block_size) {
		throw std::exception("more then max size");
	}
	_buffer.append(additional);
}



std::fstream& go_to_line(std::fstream& file, int num) {
	file.seekg(std::ios::beg);
	for (int i = 0; i < num; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return file;
}

void Stream::save(std::fstream& fout)
{
	//static const std::string place(10 + 1 + block_size, ' ');
	//go_to_line(fout, _offset);
	//fout << place;
	go_to_line(fout, _offset);
	fout << _buffer.size() << " " << _buffer << std::endl;
	_buffer.clear();
}

void Stream::load(std::fstream& fin)
{
	go_to_line(fin, _offset);
	std::getline(static_cast<std::istream&>(fin), _buffer);
}

std::string Stream::show(std::fstream& fin)
{
	std::string view;
	go_to_line(fin, _offset);
	std::getline(static_cast<std::istream&>(fin), view);
	return view;
}

std::string Stream::encrypt()
{
	std::string encrypted_str;
	for (size_t i = 0; i < _buffer.size(); i++)
	{
		auto ch = _buffer[i];
		ch += 1;
		encrypted_str += ch;
	}
	return encrypted_str;
}

std::string Stream::decrypt()
{
	std::string decrypted_str;
	for (size_t i = 0; i < _buffer.size(); i++)
	{
		auto ch = _buffer[i];
		ch -= 1;
		decrypted_str += ch;
	}
	return decrypted_str;
}
