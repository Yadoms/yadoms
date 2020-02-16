#pragma once
#include <libusbpp.hpp>
#include "Configuration.h"
#include "IDeviceManager.h"

class CDeviceManager : public IDeviceManager
{
public:

	explicit CDeviceManager(CConfiguration& configuration);
	virtual ~CDeviceManager() = default;

	std::list<std::shared_ptr<LibUSB::Device>> findDevice() const override;

	std::list<std::shared_ptr<LibUSB::Device>> getStreamDeckDevices() const override;

	boost::shared_ptr<UsbDeviceInformation> getDeviceInformation() override;

	void open();
	//virtual void reset() = 0;

private:

	CConfiguration m_configuration;
	static const uint16_t StreamDeckVendorId;
};
