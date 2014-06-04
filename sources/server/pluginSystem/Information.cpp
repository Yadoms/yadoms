#include "stdafx.h"
#include "Information.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/StringExtension.h>

namespace pluginSystem
{

//--------------------------------------------------------------
/// \brief	      Strings associated at shared::plugin::information::EReleaseType values
//--------------------------------------------------------------
typedef std::map<std::string, shared::plugin::information::EReleaseType> ReleaseTypeValuesNameList;
static const ReleaseTypeValuesNameList ReleaseTypeValuesNames = boost::assign::map_list_of
   ("stable"   , shared::plugin::information::kStable)
   ("testing"  , shared::plugin::information::kTesting)
   ("beta"     , shared::plugin::information::kBeta);


CInformation::CInformation(const boost::filesystem::path& pluginPath)
   :m_path(pluginPath)
{
   boost::property_tree::ptree propertyTree;
   try
   {
      boost::filesystem::path packageFile;
      packageFile = m_path / "package.json";
      boost::property_tree::json_parser::read_json(packageFile.string(), propertyTree);
   }
   catch (boost::property_tree::json_parser::json_parser_error& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Error reading package.json : ") + e.what());   	
   }

   try
   {
      // Get and check data

      m_name = propertyTree.get<std::string>("name");
      if (m_name.empty())
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin name can not be empty");

      m_description = propertyTree.get<std::string>("description");
      if (m_description.empty())
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin description can not be empty");

      m_version = propertyTree.get<std::string>("version");
      if (m_version.empty() || !regex_match(m_version, boost::regex("\\d+.\\d+")))
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin version doesn't match expected format (x.x)");

      ReleaseTypeValuesNameList::const_iterator it = ReleaseTypeValuesNames.find(propertyTree.get<std::string>("releaseType"));
      if (it == ReleaseTypeValuesNames.end())
      {
         std::string expectedValues;
         for (it = ReleaseTypeValuesNames.begin() ; it != ReleaseTypeValuesNames.end() ; ++ it)
         {
            expectedValues += it->first;
            ReleaseTypeValuesNameList::const_iterator it2 = it;
            if ((++it2) != ReleaseTypeValuesNames.end())
               expectedValues += ", ";
         }
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin release type doesn't match expected values (" + expectedValues + ")");
      }
      m_releaseType = (shared::plugin::information::EReleaseType)(it->second);

      m_author = propertyTree.get<std::string>("author");
      if (m_author.empty())
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin author can not be empty");

      m_url = propertyTree.get<std::string>("url", shared::CStringExtension::EmptyString);   // No check on URL
   }
   catch (boost::property_tree::ptree_bad_path& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Error reading package.json : data not found : ") + e.what());
   }
   catch (boost::property_tree::ptree_bad_data& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Error reading package.json : data is invalid : ") + e.what());
   }
}

CInformation::~CInformation()
{
}

const std::string& CInformation::getName() const
{
   return m_name;
}

const std::string& CInformation::getDescription() const
{
   return  m_description;
}

const std::string& CInformation::getVersion() const
{
   return  m_version;
}

const shared::plugin::information::EReleaseType CInformation::getReleaseType() const
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

std::string CInformation::getIdentity() const
{
   std::ostringstream formatedInformations;

   std::string releaseType;
   for (ReleaseTypeValuesNameList::const_iterator it = ReleaseTypeValuesNames.begin() ; it != ReleaseTypeValuesNames.end() ; ++ it)
      if (it->second == m_releaseType)
         releaseType = it->first;

   formatedInformations << m_name;
   formatedInformations << " v" << m_version;
   formatedInformations << "[" << releaseType << "]";

   return formatedInformations.str();
}


std::string CInformation::toString() const
{
   // Full informations = identity + author name + url
   std::ostringstream formatedInformations;

   formatedInformations << getIdentity();
   formatedInformations << " by " << m_author;
   formatedInformations << " (" << m_url << ")";

   return formatedInformations.str();
}

} // namespace pluginSystem
