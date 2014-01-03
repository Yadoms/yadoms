#include "stdafx.h"
#include "Version.h"
#include "Log.h"


CVersion::CVersion()
{
   setValues(0, 0, 0, 0);
}

CVersion::CVersion(const std::string & stringVersion)
{

   std::vector<std::string> allDigits;
   //split on slash or anti slash
   boost::split(allDigits, stringVersion, boost::is_any_of(".,"), boost::algorithm::token_compress_on);

   BOOST_FOREACH(std::string versionDigit, allDigits)
   {
         try
         {
            m_versionInfo.push_back(boost::lexical_cast<int>(versionDigit));
         }
         catch(boost::bad_lexical_cast &)
         {
            // if it throws, it's not a number.
            YADOMS_LOG(warning) << "Cannot parse version string : " << stringVersion;
            m_versionInfo.push_back(0);
         }
   }

}

CVersion::CVersion(int major, int minor, int buildNumber /*= 0*/, int revision /*= 0*/)
{
   setValues(major, minor, buildNumber, revision);
}

CVersion::CVersion(const CVersion & rhs)
{
   //clear current version info
   m_versionInfo.clear();
   //copy parameter version to current
   std::copy ( rhs.m_versionInfo.begin(), rhs.m_versionInfo.end(), m_versionInfo.begin() );
}

CVersion::~CVersion()
{
}

bool CVersion::operator<(CVersion const& rhs) const
{
   return std::lexicographical_compare(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin(), rhs.m_versionInfo.end());
}

bool CVersion::operator<=(CVersion const& rhs) const
{
   return std::lexicographical_compare(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin(), rhs.m_versionInfo.end()) ||
            std::equal(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin());
}

bool CVersion::operator>=(CVersion const& rhs) const
{
   return !std::lexicographical_compare(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin(), rhs.m_versionInfo.end());
}

bool CVersion::operator>(CVersion const& rhs) const
{
   return !std::lexicographical_compare(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin(), rhs.m_versionInfo.end()) &&
         !std::equal(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin());
}

bool CVersion::operator==(CVersion const& rhs) const
{
   return std::equal(m_versionInfo.begin(), m_versionInfo.end(), rhs.m_versionInfo.begin());
}

void CVersion::setValues(int major, int minor, int buildNumber, int revision)
{
   m_versionInfo.clear();
   m_versionInfo.push_back(major);
   m_versionInfo.push_back(minor);
   m_versionInfo.push_back(buildNumber);
   m_versionInfo.push_back(revision);
}

