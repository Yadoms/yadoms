#pragma once
#include <set>
#include <string>
#include "IDiscoveredDevice.h"

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         class CDiscoverService final
         {
         public:
            CDiscoverService() = delete;
            ~CDiscoverService() = default;


            //--------------------------------------------------------------
            /// \brief	    Discover SSDP compatible devices on local network
            /// \param[in]  url                 the url to send the request
            /// \param[in]  responseHandlerFct  lambda for response processing
            /// \param[in]  headerParameters    parameters included into the frame
            /// \param[in]  timeout             Timeout
            /// \param[in]  searchTarget        SSDP URL Pattern to search for
            /// \return     the answer of the request
            //--------------------------------------------------------------
            enum { kDefaultTimeoutSeconds = 45 };

            static std::vector<boost::shared_ptr<IDiscoveredDevice>> discover(
               const std::string& searchTarget = "ssdp:all",
               const std::chrono::duration<long long>& timeout = std::chrono::seconds(kDefaultTimeoutSeconds));

         private:
            static std::vector<boost::shared_ptr<IDiscoveredDevice>> getDevicesDescription(
               const std::set<std::string>& descriptionUrl);
         };
      }
   }
}
