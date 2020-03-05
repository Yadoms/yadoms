#include "stdafx.h"
#include "urlManagerMock.h"

urlManagerMock::urlManagerMock()
{}

shared::CDataContainerSharedPtr urlManagerMock::getRegisteredEquipments(const std::string &apikey,
                                                               const int page,
                                                               const bool activated,
                                                               const boost::posix_time::time_duration& timeout)
{
   return boost::make_shared<shared::CDataContainer>();
}

void urlManagerMock::addRegisteredEquipmentsMessageReturned(shared::CDataContainerSharedPtr newMessage)
{
   RegisteredEquipments = newMessage;
}

void urlManagerMock::addMessagesForEquipment(shared::CDataContainerSharedPtr newMessage)
{
   DeviceMessages = newMessage;
}

shared::CDataContainerSharedPtr urlManagerMock::getDeviceInformation(const std::string &apikey,
                                                            const std::string &devEUI,
                                                            const boost::posix_time::time_duration& timeout)
{
   return RegisteredEquipments;
}

shared::CDataContainerSharedPtr urlManagerMock::listDeviceCommands(const std::string &apikey,
                                                          const std::string &devEUI,
                                                          const int page,
                                                          const boost::posix_time::time_duration& timeout)
{
   return DeviceMessages;
}