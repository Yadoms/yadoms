#include "stdafx.h"
#include "Version.h"
#include <shared/exception/Exception.hpp>
#include <Poco/RegularExpression.h>
#include <Poco/NumberParser.h>

namespace shared { namespace versioning {

   CVersion::CVersion()
   {
      m_version = "0.0.0";
   }

   CVersion::CVersion(const std::string & stringVersion)
      :m_version(stringVersion)
   {

   }

   CVersion::CVersion(int major, int minor, int patch)
   {
      m_version = (boost::format("%1%.%2%.%3%") % major % minor % patch).str();
   }

   CVersion::CVersion(int major, int minor, int patch, const std::string & prerelease, const std::string & buildMetaData)
   {
      m_version = (boost::format("%1%.%2%.%3%") % major % minor % patch).str();
   }

   CVersion::CVersion(const CVersion & rhs)
   {
      m_version = rhs.m_version;
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
      // 0 : equal
      // 1 : this > rhs
      //-1 : this < rhs

      //Comparison (matching to SEMVER requirements)
      // compare major, minor and patch
      // if the same, the preRelease field (alphabetically ordered) make the precendence

      int thisMajor, thisMinor, thisPatch, rhsMajor, rhsMinor, rhsPatch;
      std::string thisPreRelease, rhsPreRelease, thisBuild, rhsBuild;

      if (!extractValues(thisMajor, thisMinor, thisPatch, thisPreRelease, thisBuild) || !rhs.extractValues(rhsMajor, rhsMinor, rhsPatch, rhsPreRelease, rhsBuild))
         throw shared::exception::CException("Fail to extract version values");

      if (thisMajor > rhsMajor)
         return 1;
      else if (thisMajor < rhsMajor)
         return -1;
      else
      {
         if (thisMinor > rhsMinor)
            return 1;
         else if (thisMinor < rhsMinor)
            return -1;
         else
         {
            if (thisPatch > rhsPatch)
               return 1;
            else if (thisPatch < rhsPatch)
               return -1;
            else
               return thisPreRelease.compare(rhsPreRelease);
         }
      }
   }

   const std::string CVersion::toString() const
   {
      return m_version;
   }



   bool CVersion::extractValues(int & major, int & minor, int & patch, std::string & prerelease, std::string & buildMetadata) const
   {
      //Regex for SEMVER is taken from http://rgxdb.com/r/40OZ1HN5
      Poco::RegularExpression re("/(?<=^[Vv]|^)(?:(?<major>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<minor>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<patch>(?:0|[1-9](?:(?:0|[1-9])+)*))(?:-(?<prerelease>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*)))*))?(?:[+](?<build>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+)))*))?)$/");
      std::vector<std::string> strings;
      int resultCount = re.split(m_version, strings);

      if (resultCount > 2)
      {
         major = Poco::NumberParser::parse(strings[0]);
         minor = Poco::NumberParser::parse(strings[1]);
         patch = Poco::NumberParser::parse(strings[2]);

         if (resultCount > 3)
         {
            prerelease = strings[3];

            if (resultCount > 4)
            {
               buildMetadata = strings[4];
            }
         }
      }

      return resultCount > 2;
   }
} } // namespace shared::versioning
