#include "stdafx.h"
#include "IpApiAutoLocation.h"
#include <shared/Log.h>
#include <shared/http/HttpMethods.h>


namespace location
{
   CIpApiAutoLocation::CIpApiAutoLocation()
   {
   }

   CIpApiAutoLocation::~CIpApiAutoLocation()
   {
   }

   void CIpApiAutoLocation::tryAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct) const
   {
      // IpApi auto location can take some time (~30 seconds) to answer if server is not joinable.
      boost::thread thread(&doAutoLocate, onFoundFct);
   }

   void CIpApiAutoLocation::doAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct)
   {
      try
      {
         auto ipApiResult(shared::CHttpMethods::SendGetRequest("http://ip-api.com/json/?fields=status,lat,lon,timezone"));

         if (ipApiResult.get("status") == std::string("fail"))
            throw shared::exception::CException(ipApiResult.get("message"));

         shared::CDataContainer foundLocation;
         foundLocation.set("latitude", ipApiResult.get<double>("lat"));
         foundLocation.set("longitude", ipApiResult.get<double>("lon"));
         foundLocation.set("altitude", 0.0); // Not available by this service
         foundLocation.set("timezone", ipApiResult.get<std::string>("timezone")); // Not available by this service

         onFoundFct(foundLocation);
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(warning) << "Unable to get location via ip-api service : " << e.what();
      }
   }
} // namespace location


