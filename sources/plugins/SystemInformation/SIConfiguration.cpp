#include "stdafx.h"
#include "SIConfiguration.h"

CSIConfiguration::CSIConfiguration()
{
   m_isAdvancedEnabled = false;
}

CSIConfiguration::~CSIConfiguration()
{
}

void CSIConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);

   try
   {
      m_isAdvancedEnabled = m_data.get<bool>("Advanced");
   }
   catch (boost::thread_interrupted&)
   {
      std::cerr << "ERROR : Plugin Configuration could not be loaded" << std::endl;
   }
}

bool CSIConfiguration::IsAdvancedEnabled() const
{
   return m_isAdvancedEnabled;
}

