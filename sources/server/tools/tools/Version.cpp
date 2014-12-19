#include "stdafx.h"
#include "Version.h"
#include <shared/Log.h>

namespace tools
{

   CVersion::CVersion()
   {
      setValues(0, 0, 0, 0);
   }

   CVersion::CVersion(const std::string & stringVersion)
   {

      std::vector<std::string> allDigits;
      //split on slash or anti slash
      boost::split(allDigits, stringVersion, boost::is_any_of(".,"), boost::algorithm::token_compress_on);

      for (std::vector<std::string>::iterator i = allDigits.begin(); i != allDigits.end(); ++i)
      {
         try
         {
            m_versionInfo.push_back(boost::lexical_cast<int>(*i));
         }
         catch (boost::bad_lexical_cast &)
         {
            // if it throws, it's not a number.
            YADOMS_LOG(warning) << "Cannot parse version string : " << stringVersion;
            m_versionInfo.push_back(0);
         }
      }

      //while(m_versionInfo.size() < 4)
      //  m_versionInfo.push_back(0);
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
      std::copy(rhs.m_versionInfo.begin(), rhs.m_versionInfo.end(), m_versionInfo.begin());
   }

   CVersion::~CVersion()
   {
   }

   bool CVersion::operator<(CVersion const& rhs) const
   {
      return (compare(rhs) < 0);
   }

   bool CVersion::operator<=(CVersion const& rhs) const
   {
      return (compare(rhs) <= 0);
   }

   bool CVersion::operator>=(CVersion const& rhs) const
   {
      return (compare(rhs) >= 0);
   }

   bool CVersion::operator>(CVersion const& rhs) const
   {
      return (compare(rhs) > 0);
   }

   bool CVersion::operator==(CVersion const& rhs) const
   {
      return (compare(rhs) == 0);
   }


   int CVersion::compare(CVersion const& rhs) const
   {
      int result = 0;
      for (unsigned int i = 0; i<m_versionInfo.size(); i++)
      {
         if (rhs.m_versionInfo.size() >= i)
         {
            if (m_versionInfo[i] == rhs.m_versionInfo[i])
               result = 0;
            else if (m_versionInfo[i] > rhs.m_versionInfo[i])
            {
               result = 1;
               break;
            }
            else
            {
               result = -1;
               break;
            }
         }
      }

      //check if a version is more precise than another
      if (m_versionInfo.size() != rhs.m_versionInfo.size() && result == 0)
      {
         if (m_versionInfo.size() > rhs.m_versionInfo.size())
         {
            //check if more precises values are only 0
            //ex : check between 1.0 and 1.0.0.0 (versions are same)
            for (unsigned int j = rhs.m_versionInfo.size(); j < m_versionInfo.size(); j++)
            {
               if (m_versionInfo[j] == 0)
                  result = 0;
               else
               {
                  result = 1;
                  break;
               }
            }
         }
         else
         {
            //check if more precises values are only 0
            //ex : check between 1.0 and 1.0.0.0 (versions are same)
            for (unsigned int j = m_versionInfo.size(); j < rhs.m_versionInfo.size(); j++)
            {
               if (rhs.m_versionInfo[j] == 0)
                  result = 0;
               else
               {
                  result = -1;
                  break;
               }
            }
         }
      }
      return result;
   }

   void CVersion::setValues(int major, int minor, int buildNumber, int revision)
   {
      m_versionInfo.clear();
      m_versionInfo.push_back(major);
      m_versionInfo.push_back(minor);
      m_versionInfo.push_back(buildNumber);
      m_versionInfo.push_back(revision);
   }


   const std::string CVersion::toString() const
   {
      std::string versionAsString;

      for (std::vector<int>::const_iterator i = m_versionInfo.begin(); i != m_versionInfo.end(); ++i)
      {
         //insert a point if there is already a digit
         if (!versionAsString.empty())
            versionAsString += ".";
         versionAsString += boost::lexical_cast<std::string>(*i);
      }

      return versionAsString;
   }

} // namespace tools