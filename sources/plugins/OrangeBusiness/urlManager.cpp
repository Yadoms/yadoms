#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>

boost::posix_time::time_duration urlManager::httpRequestCreationTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(8)));
boost::posix_time::time_duration urlManager::httpRequestWESTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(25)));

urlManager::urlManager():
   m_url("https://liveobjects.orange-business.com/api/v0") //liveobjects
{
}

shared::CDataContainer urlManager::getAllregisteredEquipments(const std::string &apikey,
                                                              const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;

   // define some hear parameters
   headerParameters.set("Host", "liveobjects.orange-business.com");
   headerParameters.set("X-API-Key", apikey);

   shared::CHttpMethods::SendGetRequest("https://liveobjects.orange-business.com/api/v0/assets",//vendors/lora/devices
                                        headerParameters,
                                        shared::CDataContainer(), // no parameters
                                        [&](shared::CDataContainer& data)
                                        {
                                           response = data;
                                        },
                                        timeout);

   return response;
}