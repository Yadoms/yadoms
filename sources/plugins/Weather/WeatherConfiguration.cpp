#include "stdafx.h"
#include "WebConnectionQualityConfiguration.h"
#include "shared/Log.h"

CWebConnectionQualityConfiguration::~CWebConnectionQualityConfiguration()
{
}

void CWebConnectionQualityConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_configuration.initializeWith(data);
}

void CWebConnectionQualityConfiguration::trace() const
{
   try
   {
      YADOMS_LOG(information) << "Configuration :";
      YADOMS_LOG(information) << "  -  periodicity (minutes) : " << periodicityInMinutes();
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}

unsigned int CWebConnectionQualityConfiguration::periodicityInMinutes() const
{
   return m_configuration.get<unsigned int>("periodicityMinutes");
}
