#include "Stream.h"

Stream::Stream(size_t offset) : _offset(offset)
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
