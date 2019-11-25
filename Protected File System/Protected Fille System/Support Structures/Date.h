#pragma once
#ifndef _DATE_H_
#define _DATE_H_
#include <fstream>

class Date
{
public:
	Date() : day(0), month(0), year(0) {}
	Date(size_t day, size_t month, size_t year) : day(day), month(month), year(year) {}
	size_t day;
	size_t month;
	size_t year;

	friend std::ostream& operator<<(std::ostream& out, const Date& date);
	friend std::ofstream& operator<<(std::ofstream& fout, const Date& date);

	friend std::ifstream& operator>>(std::ifstream& fin, Date& date);
};
#endif


inline std::ostream& operator<<(std::ostream& out, const Date& date)
{
	out << "|| Day : " << date.day << " | " << "Month : " << date.month << " | " << "Year : " << date.year << " ||";
	return out;
}

inline std::ofstream& operator<<(std::ofstream& fout, const Date& date)
{
	fout << date.day << " " << date.month << " " << date.year;
	return static_cast<std::ofstream&>(fout);
}

inline std::ifstream& operator>>(std::ifstream& fin, Date& date)
{
	fin >> date.day >> date.month >> date.year;
	return static_cast<std::ifstream&>(fin);
}
