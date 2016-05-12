#include "stdafx.h"
#include "Duration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CDuration::CDuration(std::string PluginName,
                     std::string KeyWordName)
   : m_PluginName(PluginName),
     m_duration(boost::make_shared<yApi::historization::CDuration>(KeyWordName))
{
}


void CDuration::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details) const
{
   if (!api->keywordExists(m_PluginName, m_duration->getKeyword()))
   {
      api->declareKeyword(m_PluginName, *m_duration, details);
   }
}

CDuration::~CDuration()
{
}

void CDuration::SetValue(const shared::CDataContainer& ValueContainer, const std::string& filter) const
{
   m_duration->set(ValueContainer.get<boost::posix_time::time_duration>(filter) * 24);

   std::cout << m_duration->getKeyword() << "=" << boost::posix_time::to_simple_string(m_duration->get()) << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CDuration::GetHistorizable() const
{
   return m_duration;
}

