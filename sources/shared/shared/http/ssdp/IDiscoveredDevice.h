#pragma once
#include <string>
#include "shared/DataContainer.h"

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         class IDiscoveredDevice
         {
         public:
            virtual ~IDiscoveredDevice() = default;

            //--------------------------------------------------------------
            /// \brief	    Get the device IP
            /// \return     The device IP
            //--------------------------------------------------------------	
            virtual std::string ip() const = 0;


            //--------------------------------------------------------------
            /// \brief	    Find a specific tag value from SSDP discovered device (in 'device' block)
            /// \return     Tag value or std::out_of_range exception if not found
            //--------------------------------------------------------------	
            virtual boost::shared_ptr<const CDataContainer> deviceDescription() const = 0;
         };
      }
   }
}
