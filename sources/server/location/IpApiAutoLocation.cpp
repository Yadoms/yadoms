#include "stdafx.h"
#include "IpApiAutoLocation.h"
#include <shared/Log.h>
#include <shared/http/HttpMethods.h>


namespace location
{
   void CIpApiAutoLocation::tryAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct) const
   {
      // IpApi auto location can take some time (~30 seconds) to answer if server is not reachable.
      boost::thread thread(&doAutoLocate, onFoundFct);
   }

   void CIpApiAutoLocation::doAutoLocate(boost::function<void(const shared::CDataContainer&)> onFoundFct)
   {
      try
      {
         const auto ipApiResult(shared::http::CHttpMethods::sendJsonGetRequest("http://ip-api.com/json/?fields=status,lat,lon,timezone"));
         
         if (ipApiResult->get<std::string>("status") == "fail")
            throw shared::exception::CException("ip-api.com service failed with error : " + ipApiResult->get<std::string>("message"));

         if (ipApiResult->get<std::string>("status") != "success")
            throw shared::exception::CException("ip-api.com service returned unknown error : " + ipApiResult->serialize());

         shared::CDataContainer foundLocation;
         foundLocation.set("latitude", ipApiResult->get<double>("lat"));
         foundLocation.set("longitude", ipApiResult->get<double>("lon"));
         foundLocation.set("altitude", 0.0); // Not available by this service
         foundLocation.set("timezone", ipApiResult->get<std::string>("timezone"));

         onFoundFct(foundLocation);
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(warning) << "Unable to get location via ip-api service : " << e.what();
      }
   }
} // namespace location


