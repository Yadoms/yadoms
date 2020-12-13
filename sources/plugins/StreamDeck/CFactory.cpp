#include "CFactory.h"
#include "DeviceManager.h"
#include "StreamDeckOriginal.h"
#include "StreamDeckOriginalV2.h"
#include "StreamDeckMini.h"
#include "StreamDeckXL.h"
#include "DeviceManagerHelper.h"

boost::shared_ptr<CDeviceManager> CFactory::createDeviceManager(CConfiguration& configuration,
                                                                boost::shared_ptr<CUsbDeviceInformation>&
                                                                deviceInformation,
                                                                shared::event::CEventHandler& mainEventHandler,
                                                                int evtKeyStateReceived)
{
   const auto deviceModel = CDeviceManagerHelper::getDeviceModel(deviceInformation->getProductId());

   switch (deviceModel)
   {
   case(CStreamDeckEnum::EStreamDeckType::kOriginal):
      return boost::make_shared<CStreamDeckOriginal>(configuration, mainEventHandler, evtKeyStateReceived);
   case(CStreamDeckEnum::EStreamDeckType::kOriginalV2):
      return boost::make_shared<CStreamDeckOriginalV2>(configuration, mainEventHandler, evtKeyStateReceived);
   case(CStreamDeckEnum::EStreamDeckType::kMini):
      return boost::make_shared<CStreamDeckMini>(configuration, mainEventHandler, evtKeyStateReceived);
   case(CStreamDeckEnum::EStreamDeckType::kXl):
      return boost::make_shared<CStreamDeckXL>(configuration, mainEventHandler, evtKeyStateReceived);
   default:
      throw std::runtime_error("Unknown device ");
   }
}
