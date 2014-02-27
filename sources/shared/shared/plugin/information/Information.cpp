#include "stdafx.h"
#include "Information.h"

namespace shared { namespace plugin { namespace information
{

CInformation::CInformation (const std::string & name, const std::string & description, const std::string & version,
   const EReleaseType & releaseType, const std::string & author, const std::string & url)
   :m_name(name), m_description(description), m_version(version), m_releaseType(releaseType), m_author(author), m_url(url)
{
}

CInformation::CInformation (const IInformation& src)
   :m_name(src.getName()), m_description(src.getDescription()), m_version(src.getVersion()), m_releaseType(src.getReleaseType()), m_author(src.getAuthor()), m_url(src.getUrl())
{
}

CInformation::~CInformation ()
{
}

const std::string& CInformation::getName() const
{
   return  m_name;
}

const std::string& CInformation::getDescription() const
{
   return  m_description;
}

const std::string& CInformation::getVersion() const
{
   return  m_version;
}

const EReleaseType CInformation::getReleaseType() const
{
   return m_releaseType;
}

const std::string& CInformation::getAuthor() const
{
   return m_author;
}

const std::string& CInformation::getUrl() const
{
   return  m_url;
}

std::string CInformation::toString() const
{
   std::ostringstream formatedInformations;

   formatedInformations << m_name;
   formatedInformations << " v" << m_version;
   formatedInformations << "[" << m_releaseType << "]";
   formatedInformations << " by " << m_author;
   formatedInformations << " (" << m_url << ")";

   return formatedInformations.str();
}

} } } // namespace shared::plugin::information
