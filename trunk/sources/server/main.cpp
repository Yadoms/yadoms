/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/


#include <iostream>
#include "Supervisor.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

//
// \brief Configure the logger
//
void configureLogger()
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
}

/*
\brief Main application entry point
*/
int main (int argc, char** argv)
{
    try
    {
        //comment the configureLogger to use console output
        //configureLogger();

        BOOST_LOG_TRIVIAL(info) << "Yadoms is starting";

        CSupervisor supervisor;
        supervisor.start();

        while(1)
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }

        BOOST_LOG_TRIVIAL(info) << "Yadoms is stopped.";
    }
    catch(...)
    {
        //dual logging in case logger fails/throws
        BOOST_LOG_TRIVIAL(error) << "An unhandled exception occurs. Yadoms is now stopped";
        std::cout << "An unhandled exception occurs. Yadoms is now stopped" << std::endl;
    }

    return 0;
}