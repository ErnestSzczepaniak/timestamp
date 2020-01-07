#ifndef _timestamp_h
#define _timestamp_h

static constexpr unsigned int _miliseconds_in_second = 1000;
static constexpr unsigned int _seconds_in_minute = 60;
static constexpr unsigned int _minutes_in_hour = 60;
static constexpr unsigned int _hours_in_day = 24;

static constexpr unsigned int _seconds_in_hour = _seconds_in_minute * _minutes_in_hour;
static constexpr unsigned int _seconds_in_day = _seconds_in_hour * _hours_in_day;

// ########################### TIME ##################################

class Time
{
public:
	Time();
	Time(unsigned int seconds, unsigned int miliseconds);
	~Time();

	const Time & operator++(int);
	bool operator==(const Time & time) const;
	bool operator!=(const Time & time) const;
	bool operator>(const Time & time) const;
	bool operator<(const Time & time) const;
	bool operator>=(const Time & time) const;
	bool operator<=(const Time & time) const;

	Time operator+(const Time & time);
	Time operator-(const Time & time);
	const Time & operator+=(const Time & time);
	const Time & operator-=(const Time & time);

protected:

	void _set(int seconds, int miliseconds);
	void _change(int seconds, int miliseconds);

	unsigned int _seconds;
	unsigned int _miliseconds;
};

// ########################### DURATION ##################################

class Duration
	:
	public Time
{
public:
	Duration();
	Duration(const Duration & duration);
	Duration(unsigned int miliseconds);
	Duration(unsigned int seconds, unsigned int miliseconds);
	Duration(unsigned int minutes, unsigned int seconds, unsigned int miliseconds);
	Duration(unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int miliseconds);
	Duration(unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int miliseconds);
	~Duration();

	unsigned int days() const;
	unsigned int hours() const;
	unsigned int minutes() const;
	unsigned int seconds() const;
	unsigned int miliseconds() const;

protected:

private:
};

// ########################### TIMESTAMP ##################################

class Timestamp
	:
	public Time
{
	struct Localtime { unsigned int year; unsigned int month; unsigned int day; unsigned int hour; unsigned int minute; unsigned int second; unsigned int milisecond; };

public:
	Timestamp();
	Timestamp(const Time & time);
	Timestamp(const Timestamp & timestamp);
	Timestamp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second, unsigned int milisecond);
	~Timestamp();

	unsigned int year();
	unsigned int month();
	unsigned int day();
	unsigned int day_of_year();
	unsigned int hour();
	unsigned int minute();
	unsigned int second();
	unsigned int milisecond();

protected:

	Localtime _to_localtime(Timestamp timestamp);
	Timestamp _to_timestamp(Localtime localtime);

private:

};

#endif