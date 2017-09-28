#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include "DefaultEquipment.h"

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   // Create all extensions devices
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
      std::string devEUI = "";

      // plugin state have no type
      try
      {
         devEUI = api->getDeviceDetails(*devicesIterator).get<std::string>("devEUI");
      }
      catch (...)
      {
      }

      if (devEUI != "")
         m_equipments.push_back(boost::make_shared<equipments::CDefaultEquipment>(*devicesIterator, devEUI));
   }
}

std::vector<boost::shared_ptr<equipments::IEquipment>> CDecoder::getDevices()
{
   return m_equipments;
}

CDecoder::~CDecoder()
{}

void CDecoder::decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api, 
                                    shared::CDataContainer message)
{
   auto equipments = message.get<std::vector<shared::CDataContainer> >("data");
   std::vector<shared::CDataContainer>::iterator equipmentIterator;

   for (equipmentIterator = equipments.begin(); equipmentIterator != equipments.end(); ++equipmentIterator)
   {
      std::string name = (*equipmentIterator).get<std::string>("name");
      std::string devEUI = (*equipmentIterator).get<std::string>("devEUI");
      boost::shared_ptr<equipments::CDefaultEquipment> newEquipment(boost::make_shared<equipments::CDefaultEquipment>(name, devEUI));
      m_equipments.push_back(newEquipment);
      YADOMS_LOG(information) << "create device name = " << name << " devEUI = " << devEUI;
   }
}

bool CDecoder::isFrameComplete(shared::CDataContainer message)
{
   int page = message.get<int>("page");
   int pageSize = message.get<int>("size");


   if (message.get<int>("totalCount") > (page * pageSize))
      return true;
   else
      return false;
}