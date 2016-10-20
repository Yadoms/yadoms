#include "stdafx.h"
#include "Duration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CDuration::CDuration(const std::string& pluginName,
                     const std::string& keyWordName)
   : m_pluginName(pluginName),
     m_duration(boost::make_shared<yApi::historization::CDuration>(keyWordName))
{
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

