#include "stdafx.h"
#include "SigfoxConfiguration.h"

CSigfoxConfiguration::CSigfoxConfiguration()
   :m_data(shared::CDataContainer::make())
{
}

CSigfoxConfiguration::~CSigfoxConfiguration()
{
}

void CSigfoxConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data->initializeWith(data);
}

double CSigfoxConfiguration::getRssiMin() const
{
   return m_data->get<double>("advancedConfiguration.content.rssiMin");
}

double CSigfoxConfiguration::getRssiMax() const
{
   return m_data->get<double>("advancedConfiguration.content.rssiMax");
}

double CSigfoxConfiguration::getTensionMin() const
{
   return m_data->get<double>("advancedConfiguration.content.tensionMin");
}

double CSigfoxConfiguration::getTensionMax() const
{
   return m_data->get<double>("advancedConfiguration.content.tensionMax");
}

unsigned short CSigfoxConfiguration::getSocketPort() const
{
   return m_data->get<unsigned short>("port");
}