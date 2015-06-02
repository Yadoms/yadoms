#include "stdafx.h"
#include "Information.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/StringExtension.h>
#include <boost/regex.hpp>
#include "tools/SupportedPlatformsChecker.h"

namespace pluginSystem
{

//--------------------------------------------------------------
/// \brief	      Strings associated at shared::plugin::information::EReleaseType values
//--------------------------------------------------------------
typedef std::map<std::string, shared::plugin::information::EReleaseType> ReleaseTypeValuesNameList;
static const ReleaseTypeValuesNameList ReleaseTypeValuesNames = boost::assign::map_list_of
   ("stable"   , shared::plugin::information::EReleaseType::kStable)
   ("testing"  , shared::plugin::information::EReleaseType::kTesting)
   ("beta"     , shared::plugin::information::EReleaseType::kBeta);


CInformation::CInformation(const boost::filesystem::path& pluginPath)
   :m_path(pluginPath), m_isSupportedOnThisPlatform(true)
{
   shared::CDataContainer container;
   try
   {
      boost::filesystem::path packageFile;
      packageFile = m_path / "package.json";
      container.deserializeFromFile(packageFile.string());
   }
   catch (shared::exception::CException& e)
   {
      throw shared::exception::CInvalidParameter(std::string("Error reading package.json : ") + e.what());   	
   }

   try
   {
      // Get and check data

      m_name = container.get<std::string>("name");
      if (m_name.empty())
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin name can not be empty");

	  m_description = container.get<std::string>("description");
      if (m_description.empty())
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin description can not be empty");

	  m_version = container.get<std::string>("version");
      if (m_version.empty() || !regex_match(m_version, boost::regex("\\d+.\\d+")))
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin version doesn't match expected format (x.x)");

      m_releaseType = container.get<shared::plugin::information::EReleaseType>("releaseType");

	  m_author = container.get<std::string>("author");
      if (m_author.empty())
         throw shared::exception::CInvalidParameter("Error reading package.json : plugin author can not be empty");

      if (container.containsValue("url"))
         m_url = container.get<std::string>("url");   // No check on URL
      else
         m_url = shared::CStringExtension::EmptyString;


      if (container.containsValue("supportedPlatforms") || container.containsChild("supportedPlatforms"))
         m_isSupportedOnThisPlatform = tools::CSupportedPlatformsChecker::isSupported(container.get<shared::CDataContainer>("supportedPlatforms"));
      else
         m_isSupportedOnThisPlatform = true;


      if (container.containsValue("supportManuallyDeviceCreation"))
         m_supportManuallyCreatedDevice = container.get<bool>("supportManuallyDeviceCreation");  
      else
         m_supportManuallyCreatedDevice = false;
   }
   catch (shared::exception::CException & e)
   {
      // Set plugin as not supported
      m_isSupportedOnThisPlatform = false;
      throw shared::exception::CInvalidParameter(std::string("Error reading package.json : data not found : ") + e.what());
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
   formatedInformations << m_name;
   formatedInformations << " v" << m_version;
   formatedInformations << "[" << m_releaseType << "]";
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

bool CInformation::isSupportedOnThisPlatform() const
{
   return m_isSupportedOnThisPlatform;
}

bool CInformation::getSupportManuallyCreatedDevice() const
{
   return m_supportManuallyCreatedDevice;
}

} // namespace pluginSystem
