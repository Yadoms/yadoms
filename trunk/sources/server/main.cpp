/*
 \file main.cpp

 \brief Can use "brief" tag to explicitly generate comments for file documentation.

*/
 

#include <iostream>
#include "Supervisor.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


/*
\brief Main application entry point
*/
int main (int argc, char** argv)
{
	boost::log::add_file_log
    (
        boost::log::keywords::file_name = "yadoms_%N.log",                                        
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,                                   
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), 
        boost::log::keywords::format = "[%TimeStamp%]: %Message%"                                 
    );


	boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );


	try
	{
	boost::log::add_common_attributes();

    using namespace boost::log::trivial;
    boost::log::sources::severity_logger< severity_level > lg;

		BOOST_LOG_SEV(lg, info) << "Yadoms is starting...";
		BOOST_LOG_TRIVIAL(error) << "boooooooooo !";

		CSupervisor supervisor;
		supervisor.start();

		boost::this_thread::sleep(boost::posix_time::seconds(1));
		BOOST_LOG_SEV(lg, info) << "Yadoms ends.";
	}
	catch(...)
	{
	}


	return 0;
}