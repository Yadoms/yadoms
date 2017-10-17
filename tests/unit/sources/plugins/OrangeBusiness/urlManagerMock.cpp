#include "stdafx.h"
#include "urlManagerMock.h"

urlManagerMock::urlManagerMock()
{}

shared::CDataContainer urlManagerMock::getRegisteredEquipments(const std::string &apikey,
                                                               const int page,
                                                               const bool activated,
                                                               const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer response;

   return response;
}

shared::CDataContainer urlManagerMock::getDeviceInformation(const std::string &apikey,
                                                            const std::string &devEUI,
                                                            const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer response;

   return response;
}

shared::CDataContainer urlManagerMock::listDeviceCommands(const std::string &apikey,
                                                          const std::string &devEUI,
                                                          const int page,
                                                          const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer response;

   return response;
}