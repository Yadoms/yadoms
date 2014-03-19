#include "stdafx.h"
#include "Helper.h"
#if !defined WIN32
	#include <dirent.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <cctype>
#include <boost/thread.hpp>


void StringSplit(std::string str, const std::string &delim, std::vector<std::string> &results)
{
	size_t cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos )
	{
		if(cutAt > 0)
		{
			results.push_back(str.substr(0,cutAt));
		}
		str = str.substr(cutAt+1);
	}
	if(str.length() > 0)
	{
		results.push_back(str);
	}
}

std::string stdreplace(
	std::string result, 
	const std::string& replaceWhat, 
	const std::string& replaceWithWhat)
{
	while(1)
	{
		const int pos = result.find(replaceWhat);
		if (pos==-1) break;
		result.replace(pos,replaceWhat.size(),replaceWithWhat);
	}
	return result;
}


bool file_exist (const char *filename)
{
	struct stat sbuffer;   
	return (stat(filename, &sbuffer) == 0);
}

double CalculateAltitudeFromPressure(double pressure)
{
	double seaLevelPressure=101325.0;
	double altitude = 44330.0 * (1.0 - pow( (pressure / seaLevelPressure), 0.1903));
	return altitude;
}

std::string &stdstring_ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string &stdstring_rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
std::string &stdstring_trim(std::string &s)
{
	return stdstring_ltrim(stdstring_rtrim(s));
}

double CalculateDewPoint(double temp, int humidity)
{
	if (humidity==0)
		return temp;
	double dew_numer = 243.04*(log(double(humidity)/100.0)+((17.625*temp)/(temp+243.04)));
	double dew_denom = 17.625-log(double(humidity)/100.0)-((17.625*temp)/(temp+243.04));
	if (dew_numer==0)
		dew_numer=1;
	return dew_numer/dew_denom;
}

uint32_t IPToUInt(const std::string &ip) 
{
	uint32_t a, b, c, d;
	uint32_t addr = 0;

	if (sscanf(ip.c_str(), "%3u.%3u.%3u.%3u", &a, &b, &c, &d) != 4)
		return 0;

	addr = a << 24;
	addr |= b << 16;
	addr |= c << 8;
	addr |= d;
	return addr;
}

bool isInt(const std::string &s)
{
	for(size_t i = 0; i < s.length(); i++){
		if(!isdigit(s[i]))
			return false;
	}
	return true;
}

void sleep_seconds(const long seconds)
{
#if (BOOST_VERSION < 105000)
	boost::this_thread::sleep(boost::posix_time::seconds(seconds));
#else
	boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
#endif
}

void sleep_milliseconds(const long milliseconds)
{
#if (BOOST_VERSION < 105000)
	boost::this_thread::sleep(boost::posix_time::milliseconds(milliseconds));
#else
	boost::this_thread::sleep_for(boost::chrono::milliseconds(milliseconds));
#endif
}

