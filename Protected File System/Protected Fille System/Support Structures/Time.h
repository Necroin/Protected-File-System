#pragma once
#ifndef _TIME_H
#define _TIME_H
#include <fstream>

class Time
{
public:
	Time() : minute(0), hour(0) {}
	Time(size_t hour, size_t minute) : hour(hour), minute(minute) {}
	size_t hour;
	size_t minute;

	friend std::ostream& operator<<(std::ostream& out, const Time& date);
	friend std::ofstream& operator<<(std::ofstream& fout, const Time& time);

	friend std::ifstream& operator>>(std::ifstream& fin, Time& time);
};
#endif

inline std::ostream& operator<<(std::ostream& out, const Time& time)
{
	out << "|| Hour : " << time.hour << " | " << "Minute : " << time.minute << " ||";
	return out;
}

inline std::ofstream& operator<<(std::ofstream& fout, const Time& time)
{
	fout << time.hour << " " << time.minute;
	return fout;
}

inline std::ifstream& operator>>(std::ifstream& fin, Time& time)
{
	fin >> time.hour >> time.minute;
	return fin;
}
