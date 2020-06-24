#include "stdafx.h"
#include "urlManagerMock.h"

urlManagerMock::urlManagerMock()
   : RegisteredEquipments(shared::CDataContainer::make()),
     DeviceInformation(shared::CDataContainer::make()),
     DeviceMessages(shared::CDataContainer::make())
{
}

boost::shared_ptr<shared::CDataContainer> urlManagerMock::getRegisteredEquipments(
   const std::string& apikey,
   const int page,
   const bool activated,
   int timeoutSeconds)
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

boost::shared_ptr<shared::CDataContainer> urlManagerMock::getDeviceInformation(
   const std::string& apikey,
   const std::string& devEUI,
   int timeoutSeconds)
{
   return RegisteredEquipments;
}

boost::shared_ptr<shared::CDataContainer> urlManagerMock::listDeviceCommands(
   const std::string& apikey,
   const std::string& devEUI,
   const int page,
   int timeoutSeconds)
{
   return DeviceMessages;
}
