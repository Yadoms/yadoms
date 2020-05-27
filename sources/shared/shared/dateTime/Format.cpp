#include "stdafx.h"
#include "Format.h"
#include "shared/currentTime/Provider.h"
#include <Poco/DateTimeFormatter.h>
#include "DateTimeContainer.h"

namespace shared { namespace dateTime {

	std::string CFormat::format(const Poco::DateTime& time, const std::string& boostFormat)
	{
		boost::posix_time::ptime ptime = CDateTimeContainer(time).getBoostDateTime();
		return format(ptime, boostFormat);
	}

	
	std::string CFormat::format(const boost::posix_time::ptime& time, const std::string& boostFormat)
	{
		std::stringstream s;
		std::time_t t = to_time_t(time);

		std::tm tm;
		localtime_s(&tm, &t);
		s.imbue(std::locale(""));
		s << std::put_time(&tm, boostFormat.c_str());
		return s.str();
	}

	std::string CFormat::formatNow(const std::string& boostFormat)
	{
		boost::posix_time::ptime a = shared::currentTime::Provider().now();
		return format(a, boostFormat);
	}


} } //namespace shared::dateTime