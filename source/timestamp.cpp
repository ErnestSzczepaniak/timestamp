#include "timestamp.h"

// ########################### TIME ##################################

Time::Time()
	:
	_seconds(0),
	_miliseconds(0)
{
}

Time::Time(unsigned int seconds, unsigned int miliseconds)
	:
	_seconds(seconds),
	_miliseconds(miliseconds)
{
}

Time::~Time()
{
}

const Time & Time::operator++(int)
{
	_change(0, 1);

	return *this;
}

bool Time::operator==(const Time & time) const
{
	return (_seconds == time._seconds && _miliseconds == time._miliseconds);
}

bool Time::operator!=(const Time & time) const
{
	return !(this->operator==(time));
}

bool Time::operator>(const Time & time) const
{
	if (_seconds > time._seconds)
	{
		return true;
	}
	else if (_seconds == time._seconds)
	{
		return (_miliseconds > time._miliseconds);
	}
	else
	{
		return false;
	}
}

bool Time::operator<(const Time & time) const
{
	return (!this->operator>(time) && !this->operator==(time) ? true : false);
}

bool Time::operator>=(const Time & time) const
{
	return (this->operator>(time) || this->operator==(time));
}

bool Time::operator<=(const Time & time) const
{
	return (this->operator<(time) || this->operator==(time));
}

Time Time::operator+(const Time & time)
{
	auto result = *this;

	result._change((time._seconds + (time._miliseconds / 1000)), time._miliseconds % 1000);

	return result;
}

Time Time::operator-(const Time & time)
{
	auto result = *this;

	result._change(-1 * (time._seconds + (time._miliseconds / 1000)), (-1 * time._miliseconds) % 1000);

	return result;
}

const Time & Time::operator+=(const Time & time)
{
	*this = this->operator+(time);

	return *this;
}

const Time & Time::operator-=(const Time & time)
{
	*this = this->operator-(time);

	return *this;
}

unsigned int Time::seconds()
{
	return _seconds;
}

unsigned int Time::miliseconds()
{
	return _miliseconds;
}

void Time::_set(int seconds, int miliseconds)
{
	_seconds = seconds;
	_miliseconds = miliseconds;
}

void Time::_change(int seconds, int miliseconds)
{
	if (_seconds + seconds >= 0) _seconds += seconds;
	if (_miliseconds + miliseconds >= 0 && _miliseconds + miliseconds < 1000)
	{
		_miliseconds += miliseconds;
	}
	else if (_miliseconds + miliseconds < 0)
	{
		if (_seconds > 0)
		{
			_seconds--;
			_miliseconds = _miliseconds - miliseconds + 1000;
		}
		else
		{
			_miliseconds = 0;
		}
	}
	else if (_miliseconds + miliseconds >= 1000)
	{
		_seconds++;
		_miliseconds = _miliseconds + miliseconds - 1000;
	}
}

// ########################### DURATION ##################################

Duration::Duration()
	:
	Time(0, 0)
{
}

Duration::Duration(const Duration & duration)
	:
	Time(duration._seconds, duration._miliseconds)
{
}

Duration::Duration(unsigned int miliseconds)
	:
	Time(0, miliseconds)
{
}

Duration::Duration(unsigned int seconds, unsigned int miliseconds)
	:
	Time(seconds, miliseconds)
{
}

Duration::Duration(unsigned int minutes, unsigned int seconds, unsigned int miliseconds)
	:
	Time(minutes * _seconds_in_minute + seconds, miliseconds)
{
}

Duration::Duration(unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int miliseconds)
	:
	Time(hours * _seconds_in_hour + minutes * _seconds_in_minute + seconds, miliseconds)
{
}

Duration::Duration(unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int miliseconds)
	:
	Time(days * _seconds_in_day + hours * _seconds_in_hour + minutes * _seconds_in_minute + seconds, miliseconds)
{
}

Duration::~Duration()
{
}

unsigned int Duration::days() const
{
	return (_seconds / _seconds_in_day);
}

unsigned int Duration::hours() const
{
	return (_seconds / _seconds_in_hour);
}

unsigned int Duration::minutes() const
{
	return (_seconds / _seconds_in_minute);
}

unsigned int Duration::seconds() const
{
	return (_seconds);
}

unsigned int Duration::miliseconds() const
{
	return (_seconds * _miliseconds_in_second + _miliseconds);
}

// ########################### TIMESTAMP ##################################

Timestamp::Timestamp()
	:
	Time(0, 0)
{
}

Timestamp::Timestamp(const Time & time)
	:
	Time(time)
{

}

Timestamp::Timestamp(const Timestamp & timestamp)
	:
	Time(timestamp._seconds, timestamp._miliseconds)
{
}

Timestamp::Timestamp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second, unsigned int milisecond)
	:
	Time(_to_timestamp({ year, month, day, hour, minute, second, milisecond })._seconds, milisecond)
{
}

Timestamp::~Timestamp()
{
}

unsigned int Timestamp::year()
{
	return _to_localtime(*this).year;
}

unsigned int Timestamp::month()
{
	return _to_localtime(*this).month;
}

unsigned int Timestamp::day()
{
	return _to_localtime(*this).day;
}

unsigned int Timestamp::day_of_year()
{
	auto temp = *this;
	unsigned int counter = 0;
	unsigned int current_year = _to_localtime(temp).year;

	while(_to_localtime(temp).year == current_year)
	{
		temp._seconds -= _seconds_in_day;
		counter++;
	}

	return counter;
}

unsigned int Timestamp::hour()
{
	return _to_localtime(*this).hour;
}

unsigned int Timestamp::minute()
{
	return _to_localtime(*this).minute;
}

unsigned int Timestamp::second()
{
	return _to_localtime(*this).second;
}

unsigned int Timestamp::milisecond()
{
	return _to_localtime(*this).milisecond;
}

Timestamp::Localtime Timestamp::_to_localtime(Timestamp timestamp)
{
	unsigned int z = timestamp._seconds / _seconds_in_day + 719468;
	unsigned int era = (z >= 0 ? z : z - 146096) / 146097;
	unsigned int doe = (z - era * 146097);          // [0, 146096]
	unsigned int yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	unsigned int y = yoe + era * 400;
	unsigned int doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	unsigned int mp = (5 * doy + 2) / 153;                                   // [0, 11]
	unsigned int d = doy - (153 * mp + 2) / 5 + 1;                             // [1, 31]
	unsigned int m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]

	unsigned int remaining = timestamp._seconds - (timestamp._seconds / _seconds_in_day) * _seconds_in_day;
	unsigned int hour = remaining / _seconds_in_hour;
	unsigned int mins = (remaining - hour * _seconds_in_hour) / 60;
	unsigned int sec = remaining - hour * _seconds_in_hour - mins * _seconds_in_minute;

	Localtime temp;

	temp.year = (y + (m <= 2));
	temp.month = m;
	temp.day = d;
	temp.hour = hour;
	temp.minute = mins;
	temp.second = sec;
	temp.milisecond = timestamp._miliseconds;

	return temp;
}

Timestamp Timestamp::_to_timestamp(Localtime localtime)
{
	localtime.year -= localtime.month <= 2;
	unsigned int era = (localtime.year >= 0 ? localtime.year : localtime.year - 399) / 400;
	unsigned int yoe = (localtime.year - era * 400);
	unsigned int doy = (153 * (localtime.month + (localtime.month > 2 ? -3 : 9)) + 2) / 5 + localtime.day - 1;
	unsigned int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
	unsigned int civil = era * 146097 + doe - 719468;

	Timestamp temp;

	temp._seconds = civil * _seconds_in_day + localtime.hour * _seconds_in_hour + localtime.minute * _seconds_in_minute + localtime.second;
	temp._miliseconds = localtime.milisecond;

	return temp;
}