#pragma once
#include <string>
#include "shared/DataContainer.h"

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         class CDiscoveredDevice
         {
         public:
            explicit CDiscoveredDevice(const std::string& deviceDescription);
            virtual ~CDiscoveredDevice() = default;

            //--------------------------------------------------------------
            /// \brief	    Get the device IP
            /// \return     The device IP
            //--------------------------------------------------------------	
            std::string ip() const;


            //--------------------------------------------------------------
            /// \brief	    Find a specific tag value from SSDP discovered device (in 'device' block)
            /// \param[in]  tagName             The requested tag
            /// \return     Tag value or std::out_of_range exception if not found
            //--------------------------------------------------------------	
            std::string findDeviceTag(const std::string& tagName) const;

         private:
            static boost::shared_ptr<CDataContainer> fromXml(const std::string& deviceDescription);

            const boost::shared_ptr<CDataContainer> m_description;
         };
      }
   }
}
