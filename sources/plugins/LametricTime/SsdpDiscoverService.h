#pragma once
#include <string>
#include "SsdpDiscoveredDevice.h"


class CSsdpDiscoverService final
{
public:
   CSsdpDiscoverService() = delete;
   ~CSsdpDiscoverService() = default;


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

   static std::vector<boost::shared_ptr<CSsdpDiscoveredDevice>> discover(const std::string& searchTarget = "ssdp:all",
                                                                         const std::chrono::duration<long long>& timeout
                                                                            = std::chrono::seconds(
                                                                               kDefaultTimeoutSeconds));

private:
   static std::vector<boost::shared_ptr<CSsdpDiscoveredDevice>> getDevicesDescription(
      const std::vector<std::string>& descriptionUrl);
};
