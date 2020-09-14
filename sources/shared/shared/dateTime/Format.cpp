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

		std::locale::global(std::locale(""));
        char mbstr[1024];
		memset(mbstr, 0, sizeof(mbstr));
		strftime(mbstr, sizeof(mbstr), boostFormat.c_str(), std::localtime(&t));
		s << mbstr;
		return s.str();

		/* c++11 equivalent  (but some gcc <5 do not handle std::put_time)
		s.imbue(std::locale(""));
		s << std::put_time(localtime(&t), boostFormat.c_str());
		*/
	}

	std::string CFormat::formatNow(const std::string& boostFormat)
	{
		boost::posix_time::ptime a = shared::currentTime::Provider().now();
		return format(a, boostFormat);
	}


} } //namespace shared::dateTime