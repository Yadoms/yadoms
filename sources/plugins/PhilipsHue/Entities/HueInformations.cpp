#include "HueInformations.h"

CHueInformations::CHueInformations()
{
}

CHueInformations::CHueInformations(std::string& ip, std::string& friendlyName, std::string& modelName,
                                   std::string& modelNumber)
   : m_ip(ip),
     m_friendlyName(friendlyName),
     m_modelName(modelName),
     m_modelNumber(modelNumber)
{
}

const std::string& CHueInformations::getIp() const
{
   return m_ip;
}

const std::string& CHueInformations::getFriendlyName() const
{
   return m_friendlyName;
}

const std::string& CHueInformations::getModelName() const
{
   return m_modelName;
}

const std::string& CHueInformations::getModelNumber() const
{
   return m_modelNumber;
}

void CHueInformations::setIp(const std::string& ip)
{
   m_ip = ip;
}

void CHueInformations::setFriendlyName(const std::string& friendlyName)
{
   m_friendlyName = friendlyName;
}

void CHueInformations::setModelName(const std::string& modelName)
{
   m_modelName = modelName;
}

void CHueInformations::setModelNumber(const std::string& modelNumber)
{
   m_modelNumber = modelNumber;
}
