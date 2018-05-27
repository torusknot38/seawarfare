//Troy Moench
#pragma warning(disable: 4996)

#include "ATime.h"
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

ATime::ATime()
{
	m_time = 0;
}

ATime::ATime(time_t x)
{
	m_time = x;
}

ATime::ATime(string mdy, string hms)
{
	int mon, day, yr, hr, min, sec;
	char junk;
	istringstream is(mdy);
	is >> mon >> junk >> day >> junk >> yr;
	istringstream is2(hms);
	is2 >> hr >> junk >> min >> junk >> sec;
	struct tm present;
	present.tm_mon = mon - 1;
	present.tm_mday = day;
	present.tm_year = yr - 1900;
	present.tm_hour = hr;
	present.tm_min = min;
	present.tm_sec = sec;
	present.tm_isdst = 0;
	m_time = mktime(&present);
}

string ATime::getSVal() const
{
	struct tm now;
	now = *localtime(&m_time);
	ostringstream os;
	int mon = now.tm_mon + 1;
	int day = now.tm_mday;
	int yr = now.tm_year + 1900;
	int hr = now.tm_hour;
	int min = now.tm_min;
	int sec = now.tm_sec;
	os.fill('0');
	os << setw(2) << mon << "/"
		<< setw(2) << day << "/"
		<< yr << " "
		<< setw(2) << hr << ":"
		<< setw(2) << min << ":"
		<< setw(2) << sec;
	return os.str();
}

bool ATime::isValid() const
{
	return m_time > 0;
}

bool ATime::operator<(const ATime & b) const
{
	return m_time < b.m_time;
}

bool ATime::operator>(const ATime & b) const
{
	return m_time > b.m_time;
}

bool ATime::operator==(const ATime & b) const
{
	return m_time == b.m_time;
}

bool ATime::operator!=(const ATime & b) const
{
	return m_time != b.m_time;
}

ATime ATime::operator+(const int i) const
{
	ATime temp(m_time);
	temp.m_time = m_time + i;
	return temp;
}

ATime ATime::operator++() //preincrement
{
	m_time++;
	return *this;
}

ATime ATime::operator++(int) //postincrement
{
	ATime temp(m_time);
	m_time++;
	return temp;
}

ATime& ATime::operator+=(int i)
{
	m_time += i;
	return *this;
}

int ATime::operator-(const ATime& b) const
{
	return m_time - b.m_time;
}

ATime ATime::operator-(const int i) const
{
	ATime temp(m_time);
	temp.m_time = m_time - i;
	return temp;
}

ATime operator+(int i, const ATime& t)
{
	return t + i;
}

ostream& operator<<(ostream& os, ATime at)
{
	os << at.getSVal();
	return os;
}