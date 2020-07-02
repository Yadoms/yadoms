#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>

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
            /// \brief	    Get the XML content
            /// \return     XML content, returned byt the discovered device
            //--------------------------------------------------------------	
            virtual boost::shared_ptr<const boost::property_tree::ptree> xmlContent() const = 0;
         };
      }
   }
}
