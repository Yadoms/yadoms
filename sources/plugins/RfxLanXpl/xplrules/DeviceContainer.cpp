#include "stdafx.h"
#include "DeviceContainer.h"

namespace xplrules
{
   CDeviceContainer::CDeviceContainer(const CDeviceIdentifier& deviceIdentifier)
      : m_deviceIdentifier(deviceIdentifier)
   {
   }

   CDeviceContainer::~CDeviceContainer()
   {
   }


   void CDeviceContainer::addKeyword(boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> keyword)
   {
      m_keywords.push_back(keyword);
   }

   const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >& CDeviceContainer::getKeywords() const
   {
      return m_keywords;
   }

   const CDeviceIdentifier& CDeviceContainer::getDeviceIdentifier() const
   {
      return m_deviceIdentifier;
   }
} //namespace xplrules


