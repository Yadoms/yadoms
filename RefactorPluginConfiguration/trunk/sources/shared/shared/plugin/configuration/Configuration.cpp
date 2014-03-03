#include "stdafx.h"

#include "../../exception/BadConversion.hpp"
#include "../../exception/OutOfRange.hpp"
#include "../../StringExtension.h"
#include "Configuration.h"
#include "Factory.h"
#include <shared/exception/Exception.hpp>
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace configuration
{

CConfiguration::CConfiguration()
   :m_configurationSerializer(CFactory::createSerializer())
{

}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::set(const std::string& serializedConfiguration)
{
   boost::lock_guard<boost::mutex> lock(m_treeMutex);

   try
   {
      m_configurationSerializer->deserialize(serializedConfiguration, m_tree);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Error loading configuration : " << e.what();
      throw shared::exception::CException(std::string("Error loading configuration : ") + e.what());
   }
}

} } } // namespace shared::plugin::configuration
