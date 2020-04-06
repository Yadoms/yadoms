#pragma once
#include <libusbpp.hpp>
#include "Configuration.h"
#include <hidapi.h>
#include "shared/event/EventHandler.hpp"

class CDeviceManager
{
public:
	
	typedef boost::function<int()> OnKeyPressedCallback;
	CDeviceManager(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived);

	virtual ~CDeviceManager();

	void open();

	void close();
	
	void runKeyStateThread();

	void readHandler();

	virtual void reset() = 0;

	virtual void resetKeyStream() = 0;

	virtual void setBrightness(int percent) = 0;

	virtual void setKeyImage(std::string& content, int& keyIndex, std::string& customText) = 0;

	virtual std::pair<bool, int> readKeyStates() = 0;
private:

	CConfiguration m_configuration;
	
	static const uint16_t StreamDeckVendorId;

	boost::shared_ptr<boost::thread>  m_readKeyThread;
	shared::event::CEventHandler& m_mainEventHandler;
	int m_mainEvtKeyStateReceived;

protected:

	hid_device* m_handle;
};
