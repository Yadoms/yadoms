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
   enum { DefaultTimeoutSeconds = 45 };
   static CSsdpDiscoveredDevice discover(const std::string& searchTarget = "ssdp:all",
                                         const std::chrono::duration<long long>& timeout = std::chrono::seconds(
                                            DefaultTimeoutSeconds));

private:
   static std::vector<std::string> getDevicesDescription(const std::vector<std::string>& descriptionUrl);
};
