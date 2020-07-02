#pragma once
#include "IDiscoveredDevice.h"

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         class CDiscoveredDevice : public IDiscoveredDevice
         {
         public:
            explicit CDiscoveredDevice(const std::string& xmlContent);
            virtual ~CDiscoveredDevice() = default;

            // IDiscoveredDevice implementation
            boost::shared_ptr<const boost::property_tree::ptree> xmlContent() const override;
            // [END] IDiscoveredDevice implementation

         private:
            static boost::shared_ptr<const boost::property_tree::ptree> contentFromXml(
               const std::string& xmlContent);

            boost::shared_ptr<const boost::property_tree::ptree> m_xmlContent;
         };
      }
   }
}
