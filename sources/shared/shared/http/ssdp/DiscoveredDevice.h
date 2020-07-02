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
            CDiscoveredDevice(const std::string& location,
                              const std::string& deviceDescription);
            virtual ~CDiscoveredDevice() = default;

            //--------------------------------------------------------------
            /// \brief	    Get the device IP
            /// \return     The device IP
            //--------------------------------------------------------------	
            std::string ip() const override;


            //--------------------------------------------------------------
            /// \brief	    Find a specific tag value from SSDP discovered device (in 'device' block)
            /// \return     Tag value or std::out_of_range exception if not found
            //--------------------------------------------------------------	
            boost::shared_ptr<const CDataContainer> deviceDescription() const override;

         private:
            static std::string ipFromXml(const std::string& deviceDescription);
            static boost::shared_ptr<const CDataContainer> deviceDescriptionFromXml(
               const std::string& deviceDescription);

            const std::string& m_location;
            const std::string m_ip;
            boost::shared_ptr<const CDataContainer> m_deviceDescription;
         };
      }
   }
}
