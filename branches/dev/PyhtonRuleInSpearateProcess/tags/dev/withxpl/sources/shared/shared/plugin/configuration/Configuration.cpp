#include "stdafx.h"

#include "../../exception/Exception.hpp"
#include "../../exception/InvalidParameter.hpp"
#include "../../StringExtension.h"
#include "../../Log.h"
#include "Configuration.h"
#include "Factory.h"


namespace shared { namespace plugin { namespace configuration
{

CConfiguration::CConfiguration()
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
      boost::shared_ptr<serialization::IPtreeToStringSerializer> configurationSerializer(CFactory::createSerializer());
      configurationSerializer->deserialize(serializedConfiguration, m_tree);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Error loading configuration : " << e.what();
      throw shared::exception::CException(std::string("Error loading configuration : ") + e.what());
   }
}

} } } // namespace shared::plugin::configuration
