#include "stdafx.h"
#include "Version.h"
#include <shared/exception/Exception.hpp>
#include <Poco/RegularExpression.h>
#include <Poco/NumberParser.h>
#include <regex>

namespace shared
{
   namespace versioning
   {
      CVersion::CVersion()
         : m_major(0), m_minor(0), m_patch(0), m_prerelease(""), m_buildMetadata("")
      {
      }

      CVersion::CVersion(const std::string& stringVersion)
      {
         parse(stringVersion);
      }

      CVersion::CVersion(int major, int minor, int patch)
         : m_major(major), m_minor(minor), m_patch(patch), m_prerelease(""), m_buildMetadata("")
      {
      }

      CVersion::CVersion(int major, int minor, int patch, const std::string& prerelease)
         : m_major(major), m_minor(minor), m_patch(patch), m_prerelease(prerelease), m_buildMetadata("")
      {
      }

      CVersion::CVersion(int major, int minor, int patch, const std::string& prerelease, const std::string& buildMetadata)
         : m_major(major), m_minor(minor), m_patch(patch), m_prerelease(prerelease), m_buildMetadata(buildMetadata)
      {
      }

      CVersion::CVersion(const CVersion& rhs)
         : m_major(rhs.m_major), m_minor(rhs.m_minor), m_patch(rhs.m_patch), m_prerelease(rhs.m_prerelease), m_buildMetadata(rhs.m_buildMetadata)
      {
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

      int CVersion::major() const
      {
         return m_major;
      }

      int CVersion::minor() const
      {
         return m_minor;
      }

      int CVersion::patch() const
      {
         return m_patch;
      }

      const std::string& CVersion::prerelease() const
      {
         return m_prerelease;
      }

      const std::string& CVersion::buildMetadata() const
      {
         return m_buildMetadata;
      }

      int CVersion::compare(CVersion const& rhs) const
      {
         // 0 : equal
         // 1 : this > rhs
         //-1 : this < rhs

         //Comparison (matching to SEMVER requirements)
         // compare major, minor and patch
         // if the same, the preRelease field (alphabetically ordered) make the precendence

         if (m_major > rhs.m_major)
            return 1;
         if (m_major < rhs.m_major)
            return -1;
         if (m_minor > rhs.m_minor)
            return 1;
         if (m_minor < rhs.m_minor)
            return -1;
         if (m_patch > rhs.m_patch)
            return 1;
         if (m_patch < rhs.m_patch)
            return -1;
         if (m_prerelease.empty() && rhs.m_prerelease.empty())
            return 0;
         if (m_prerelease.empty() && !rhs.m_prerelease.empty())
            return 1;
         if (!m_prerelease.empty() && rhs.m_prerelease.empty())
            return -1;
         return m_prerelease.compare(rhs.m_prerelease);
      }

      std::string CVersion::toString() const
      {
         auto prerelease = m_prerelease.empty() ? "" : ("-" + m_prerelease);
         auto buildMetadata = m_buildMetadata.empty() ? "" : ("+" + m_buildMetadata);
         return (boost::format("%1%.%2%.%3%%4%%5%") % m_major % m_minor % m_patch % prerelease % buildMetadata).str();
      }


      void CVersion::parse(const std::string& version)
      {
         //Regex for SEMVER is taken from http://rgxdb.com/r/40OZ1HN5 (adjusted by removing leading and trailing "/" )
         Poco::RegularExpression re("(?<=^[Vv]|^)(?:(?<major>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<minor>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<patch>(?:0|[1-9](?:(?:0|[1-9])+)*))(?:-(?<prerelease>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*)))*))?(?:[+](?<build>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+)))*))?)$");
         std::vector<std::string> strings;
         auto resultCount = re.split(version, strings);

         if (resultCount > 3)
         {
            //std::string foundStr(ss.str().substr(match.offset, match.length));

            m_major = Poco::NumberParser::parse(strings[1]);
            m_minor = Poco::NumberParser::parse(strings[2]);
            m_patch = Poco::NumberParser::parse(strings[3]);

            if (resultCount > 4)
            {
               m_prerelease = strings[4];

               if (resultCount > 5)
               {
                  m_buildMetadata = strings[5];
               }
            }
         }
         else
         {
            throw exception::CException("Fail to parse version from " + version);
         }
      }
   }
} // namespace shared::versioning
