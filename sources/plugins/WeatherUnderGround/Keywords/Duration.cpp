#include "stdafx.h"
#include "Duration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CDuration::CDuration(std::string pluginName,
                     std::string keyWordName)
   : m_pluginName(pluginName),
     m_duration(boost::make_shared<yApi::historization::CDuration>(keyWordName))
{
}


void CDuration::initialize(boost::shared_ptr<yApi::IYPluginApi> api,
   shared::CDataContainer details) const
{
   if (!api->keywordExists(m_pluginName, m_duration))
      api->declareKeyword(m_pluginName, m_duration, details);
}

CDuration::~CDuration()
{
}

void CDuration::setValue(const shared::CDataContainer& valueContainer,
   const std::string& filter) const
{
   m_duration->set(valueContainer.get<boost::posix_time::time_duration>(filter) * 24);

   std::cout << m_duration->getKeyword() << "=" << boost::posix_time::to_simple_string(m_duration->get()) << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CDuration::getHistorizable() const
{
   return m_duration;
}

