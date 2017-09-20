#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>

urlManager::urlManager():
   m_url("https://liveobjects.orange-business.com/api/v0"), //liveobjects
   m_baseUrl("liveobjects.orange-business.com")
{
}

shared::CDataContainer urlManager::getAllregisteredEquipments(const std::string &apikey,
                                                              const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;

   // define some hear parameters
   headerParameters.set("Host", m_baseUrl.str());
   headerParameters.set("X-API-Key", apikey);

   shared::CHttpMethods::SendGetSecureRequest("https://liveobjects.orange-business.com/api/v0/vendors/lora/devices",//assets
                                              headerParameters,
                                              shared::CDataContainer(), // no parameters
                                              [&](shared::CDataContainer& data)
                                              {
                                                 response = data;
                                              },
                                              timeout);

   return response;
}