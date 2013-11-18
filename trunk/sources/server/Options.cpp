//
// Options.cpp
//
#pragma once

#include <Options.h>
#include <boost/program_options.hpp>


COptions::m_instance = NULL;

COptions::COptions()
{
   loadConfiguration();
}

COptions* COptions::instance()
{
   if (m_instance == NULL)
      m_instance = new COptions;
   return m_instance;
}

void COptions::loadConfiguration()
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()
      ("help", "produce help message")
      ("logLevel", boost::program_options::value<int>(), "set compression level")
      ;
}

boost::log::trivial::severity_level COptions::getLogLevel()
{
   instance()->
}
