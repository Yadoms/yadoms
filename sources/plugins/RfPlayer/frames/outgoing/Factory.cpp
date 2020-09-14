#include "stdafx.h"
#include "Factory.h"
#include "OutgoingFrame.h"

namespace frames {
namespace outgoing {
   
   boost::shared_ptr<IOutgoingFrame> CFactory::make(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
   {
         boost::shared_ptr<shared::CDataContainer> details = api->getDeviceDetails(command->getDevice());
         if (details->get<std::string>("protocol") == "blyss")
         {
            return boost::make_shared<COutgoingFrame>("BLYSS");
         }
         if (details->get<std::string>("protocol") == "visonic" && details->get<std::string>("frequency") == "433")
         {
            return boost::make_shared<COutgoingFrame>("VISONIC433");
         }
         if (details->get<std::string>("protocol") == "visonic" && details->get<std::string>("frequency") == "868")
         {
            return boost::make_shared<COutgoingFrame>("VISONIC868");
         }
         if (details->get<std::string>("protocol") == "chacon")
         {
            return boost::make_shared<COutgoingFrame>("CHACON");
         }
         if (details->get<std::string>("protocol") == "domia")
         {
            return boost::make_shared<COutgoingFrame>("DOMIA");
         }
         if (details->get<std::string>("protocol") == "x10")
         {
            return boost::make_shared<COutgoingFrame>("X10");
         }
         if (details->get<std::string>("protocol") == "rts")
         {
            return boost::make_shared<COutgoingFrame>("RTS");
         }
         if (details->get<std::string>("protocol") == "parrot")
         {
            return boost::make_shared<COutgoingFrame>("PARROT");
         }
         if (details->get<std::string>("protocol") == "kd101")
         {
            return boost::make_shared<COutgoingFrame>("KD101");
         }
         if (details->get<std::string>("protocol") == "x2d")
         {
            if(details->get<std::string>("x2dType") == "shutter")
               return boost::make_shared<COutgoingFrame>("X2DSHUTTER");
            if(details->get<std::string>("x2dType") == "elec")
               return boost::make_shared<COutgoingFrame>("X2DELEC");
            if(details->get<std::string>("x2dType") == "gas")
               return boost::make_shared<COutgoingFrame>("X2DGAS");
            if(details->get<std::string>("frequency") == "433")
               return boost::make_shared<COutgoingFrame>("X2D433");
            if(details->get<std::string>("frequency") == "868")
               return boost::make_shared<COutgoingFrame>("X2D868");
         }
         
         return boost::shared_ptr<IOutgoingFrame>();
   }
   

      
} //namespace incoming
} //namespace frames