#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include "DefaultEquipment.h"

CDecoder::CDecoder()
{}

CDecoder::~CDecoder()
{}

std::map<std::string, boost::shared_ptr<equipments::IEquipment>> CDecoder::decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                    shared::CDataContainer& message)
{
	std::map<std::string, boost::shared_ptr<equipments::IEquipment>> equipmentList;
   auto equipments = message.get<std::vector<shared::CDataContainer> >("data");
   std::vector<shared::CDataContainer>::iterator equipmentIterator;

   for (equipmentIterator = equipments.begin(); equipmentIterator != equipments.end(); ++equipmentIterator)
   {
      std::string name = (*equipmentIterator).get<std::string>("name");
      std::string devEUI = (*equipmentIterator).get<std::string>("devEUI");
      boost::shared_ptr<equipments::CDefaultEquipment> newEquipment(boost::make_shared<equipments::CDefaultEquipment>(name, devEUI));
	  equipmentList.insert(std::pair<std::string, boost::shared_ptr<equipments::IEquipment>>(name, newEquipment));
      YADOMS_LOG(information) << "create device name = " << name << " devEUI = " << devEUI;
   }

   return equipmentList;
}

bool CDecoder::isFrameComplete(shared::CDataContainer& message)
{
   int page = message.get<int>("page");
   int pageSize = message.get<int>("size");

   if (message.get<int>("totalCount") > (page * pageSize))
      return true;
   else
      return false;
}

shared::CDataContainer CDecoder::getLastData(shared::CDataContainer& message)
{
   auto commands = message.get<std::vector<shared::CDataContainer> >("data");
   shared::CDataContainer response;

   if (commands.size() > 0)
   {
      response.set("data", commands[commands.size() - 1].get<std::string>("data"));
	  response.set("date", commands[commands.size() - 1].get<std::string>("creationTs"));
   }
   return response;
}