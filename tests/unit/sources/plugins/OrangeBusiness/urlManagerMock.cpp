#include "stdafx.h"
#include "urlManagerMock.h"

urlManagerMock::urlManagerMock()
{}

boost::shared_ptr<shared::CDataContainer> urlManagerMock::getRegisteredEquipments(const std::string &apikey,
                                                               const int page,
                                                               const bool activated,
                                                               const boost::posix_time::time_duration& timeout)
{
   return boost::make_shared<shared::CDataContainer>();
}

void urlManagerMock::addRegisteredEquipmentsMessageReturned(boost::shared_ptr<shared::CDataContainer> newMessage)
{
   RegisteredEquipments = newMessage;
}

void urlManagerMock::addMessagesForEquipment(boost::shared_ptr<shared::CDataContainer> newMessage)
{
   DeviceMessages = newMessage;
}

boost::shared_ptr<shared::CDataContainer> urlManagerMock::getDeviceInformation(const std::string &apikey,
                                                            const std::string &devEUI,
                                                            const boost::posix_time::time_duration& timeout)
{
   return RegisteredEquipments;
}

boost::shared_ptr<shared::CDataContainer> urlManagerMock::listDeviceCommands(const std::string &apikey,
                                                          const std::string &devEUI,
                                                          const int page,
                                                          const boost::posix_time::time_duration& timeout)
{
   return DeviceMessages;
}