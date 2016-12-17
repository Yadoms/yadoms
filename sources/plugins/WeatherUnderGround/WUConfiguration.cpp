#include "stdafx.h"
#include "WUConfiguration.h"
#include "WeatherUndergroundHelpers.h"
#include"NoStateException.hpp"
#include "Features/Location.h"

CWUConfiguration::CWUConfiguration()
{
}

CWUConfiguration::~CWUConfiguration()
{
}

void CWUConfiguration::initializeWith(const shared::CDataContainer& data)
{
   try
   {
      m_data.initializeWith(data);
   }
   catch (boost::thread_interrupted&)
   {
      std::cerr << "ERROR : Plugin Configuration could not be loaded" << std::endl;
   }
}

std::string CWUConfiguration::getAPIKey() const
{
   return m_data.get<std::string>("APIKey");
}

bool CWUConfiguration::pluginLocationEnabled() const
{
   return m_data.get<bool>("location.checkbox");
}

boost::shared_ptr<const shared::ILocation> CWUConfiguration::getLocation() const
{
   boost::shared_ptr<const shared::ILocation> location;

   return boost::make_shared<location::CLocation>(m_data.get<double>("location.content.longitude"),
                                                  m_data.get<double>("location.content.latitude"),
                                                  0);
}