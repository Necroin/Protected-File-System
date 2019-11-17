#pragma once
#ifndef _DATE_H_
#define _DATE_H_

class Date
{
public:
	Date() : day(0), month(0), year(0) {}
	Date(size_t day, size_t month, size_t year) : day(day), month(month), year(year) {}
	size_t day;
	size_t month;
	size_t year;

	template <class _Output>
	friend _Output& operator<<(_Output& out, const Date& date);
};
#endif

template<class _Output>
inline _Output& operator<<(_Output& out, const Date& date)
{
	out << "|| Day : " << date.day << " | " << "Month : " << date.month << " | " << "Year : " << date.year << " ||";
	return out;
}
