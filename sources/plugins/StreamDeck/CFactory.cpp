#include "CFactory.h"
#include "DeviceManager.h"
#include "StreamDeckOriginal.h"
#include "StreamDeckOriginalV2.h"
#include "StreamDeckMini.h"
#include "StreamDeckXL.h"


boost::shared_ptr<CDeviceManager> CFactory::createDeviceManager(CConfiguration& configuration,
                                                                boost::shared_ptr<UsbDeviceInformation>&
                                                                deviceInformation,
                                                                shared::event::CEventHandler& mainEventHandler,
                                                                int evtKeyStateReceived)
{
	const auto deviceModel = CDeviceManagerHelper::getDeviceModel(deviceInformation->productID);

	switch (deviceModel)
	{
	case(CStreamDeckFactory::EStreamDeckModel::kOriginal):
		return boost::make_shared<CStreamDeckOriginal>(configuration, mainEventHandler, evtKeyStateReceived);
	case(CStreamDeckFactory::EStreamDeckModel::kOriginalV2):
		return boost::make_shared<CStreamDeckOriginalV2>(configuration, mainEventHandler, evtKeyStateReceived);
	case(CStreamDeckFactory::EStreamDeckModel::kMini):
		return boost::make_shared<CStreamDeckMini>(configuration, mainEventHandler, evtKeyStateReceived);
	case(CStreamDeckFactory::EStreamDeckModel::kXl):
		return boost::make_shared<CStreamDeckXL>(configuration, mainEventHandler, evtKeyStateReceived);
	default:
		throw std::runtime_error("Unknown device ");
	}
}
