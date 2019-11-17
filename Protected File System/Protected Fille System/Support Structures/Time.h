#pragma once
#ifndef _TIME_H
#define _TIME_H

class Time
{
public:
	Time() : minute(0), hour(0) {}
	Time(size_t hour, size_t minute) : hour(hour), minute(minute) {}
	size_t hour;
	size_t minute;

	template <class _Output>
	friend _Output& operator<<(_Output& out, const Time& date);
};
#endif

template<class _Output>
inline _Output& operator<<(_Output& out, const Time& time)
{
	out << "|| Hour : " << time.hour << " | " << "Minute : " << time.minute << " ||";
	return out;
}
