#include "stdafx.h"
#include "Information.h"



CHardwarePluginInformation::CHardwarePluginInformation (const std::string & name, const std::string & version,
   const EReleaseType & releaseType, const std::string & author, const std::string & url)
   :m_name(name), m_version(version), m_releaseType(releaseType), m_author(author), m_url(url)
{
}

CHardwarePluginInformation::CHardwarePluginInformation (const IHardwarePluginInformation& src)
   :m_name(src.getName()), m_version(src.getVersion()), m_releaseType(src.getReleaseType()), m_author(src.getAuthor()), m_url(src.getUrl())
{
}

CHardwarePluginInformation::~CHardwarePluginInformation ()
{
}

const std::string& CHardwarePluginInformation::getName() const
{
   return  m_name;
}

const std::string& CHardwarePluginInformation::getVersion() const
{
   return  m_version;
}

const IHardwarePluginInformation::EReleaseType CHardwarePluginInformation::getReleaseType() const
{
   return m_releaseType;
}

const std::string& CHardwarePluginInformation::getAuthor() const
{
   return m_author;
}

const std::string& CHardwarePluginInformation::getUrl() const
{
   return  m_url;
}

std::string CHardwarePluginInformation::toString() const
{
   std::ostringstream formatedInformations;

   formatedInformations << m_name;
   formatedInformations << " v" << m_version;
   formatedInformations << "[" << m_releaseType << "]";
   formatedInformations << " by " << m_author;
   formatedInformations << " (" << m_url << ")";

   return formatedInformations.str();
}
