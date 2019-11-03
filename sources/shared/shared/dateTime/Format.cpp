#include "stdafx.h"
#include "Format.h"
#include "shared/currentTime/Provider.h"
#include <Poco/DateTimeFormatter.h>
#include "DateTimeContainer.h"
#include <boost/locale/info.hpp>
#include <boost/locale/generator.hpp>

namespace shared { namespace dateTime {

	std::string CFormat::format(Poco::DateTime& time, const std::string& boostFormat)
	{
		boost::posix_time::ptime ptime = CDateTimeContainer(time).getBoostDateTime();
		return format(ptime, boostFormat);
	}

	std::string CFormat::format(boost::posix_time::ptime& time, const std::string& boostFormat)
	{
		boost::posix_time::time_facet* facet = new boost::posix_time::time_facet(boostFormat.c_str());
		std::stringstream s;
		boost::locale::generator gen;
		std::locale l = gen(""); //get system locale
		std::locale lf(l, facet); //create system local with facet
		s.imbue(lf); //apply to current stream
		s << time;
		return s.str();
	}

	std::string CFormat::formatNow(const std::string& boostFormat)
	{
		boost::posix_time::ptime a = shared::currentTime::Provider().now();
		return format(a, boostFormat);
	}


} } //namespace shared::dateTime