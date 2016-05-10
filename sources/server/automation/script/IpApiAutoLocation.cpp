#include "stdafx.h"
#include "IpApiAutoLocation.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/http/HttpMethods.h>


namespace automation { namespace script
{

CIpApiAutoLocation::CIpApiAutoLocation()
{
}

CIpApiAutoLocation::~CIpApiAutoLocation()
{         
}

shared::CDataContainer CIpApiAutoLocation::tryAutoLocate() const
{
   try
   {
      shared::CDataContainer foundLocation;

      shared::CDataContainer ipApiResult(shared::CHttpMethods::SendGetRequest("http://ip-api.com/json/?fields=lat,lon"));

      if (ipApiResult.get("status") == std::string("fail"))
         throw shared::exception::CException(ipApiResult.get("message"));

      foundLocation.set("latitude", ipApiResult.get<double>("lat"));
      foundLocation.set("longitude", ipApiResult.get<double>("lon"));
      foundLocation.set("altitude", 0.0); // Not available by this service

      return foundLocation;
   }
   catch (shared::exception::CException& e)
   {
      throw shared::exception::CEmptyResult(std::string("Unable to get location via ip-api service : ") + e.what());
   }
}

} } // namespace automation::script

