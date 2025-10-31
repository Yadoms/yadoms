#pragma once
#include "IDevice.h"
#include <shared/DataContainer.h>

namespace hardware::usb
{
	class IDevicesLister
	{
	public:
		virtual ~IDevicesLister() = default;

		virtual std::vector<boost::shared_ptr<IDevice>> listUsbDevices() = 0;
	};
}