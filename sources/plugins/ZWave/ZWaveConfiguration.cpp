#include "stdafx.h"
#include "ZWaveConfiguration.h"


CZWaveConfiguration::CZWaveConfiguration()
   :m_container(shared::CDataContainer::make())
{
}

CZWaveConfiguration::~CZWaveConfiguration()
{
}

void CZWaveConfiguration::initializeWith(boost::shared_ptr<shared::CDataContainer> container)
{
   m_container->initializeWith(container);
}


std::string CZWaveConfiguration::getSerialPort() const
{
   return m_container->get<std::string>("Serial Port");
}

bool CZWaveConfiguration::getIncludeSystemKeywords() const
{
   return m_container->getWithDefault<bool>("IncludeSystemKeywords", false);
}

std::string CZWaveConfiguration::getPath() const
{
   return m_container->get<std::string>("libraryPath");
}

void CZWaveConfiguration::setPath(const std::string & path)
{
   m_container->set("libraryPath", path);
}

std::string CZWaveConfiguration::getDataPath() const
{
   return m_container->get<std::string>("dataPath");
}

void CZWaveConfiguration::setDataPath(const std::string & dataPath)
{
   m_container->set("dataPath", dataPath);
}

int CZWaveConfiguration::getInitTimeout() const
{
   return m_container->getWithDefault<int>("initTimeout", 0);
}

void CZWaveConfiguration::setInitTimeout(int timeOut)
{
   m_container->set("initTimeout", timeOut);
}