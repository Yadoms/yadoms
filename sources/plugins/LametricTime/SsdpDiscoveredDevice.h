#pragma once
#include <string>
#include "shared/DataContainer.h"

class CSsdpDiscoveredDevice
{
public:
	explicit CSsdpDiscoveredDevice(const std::string& deviceDescription);
	virtual ~CSsdpDiscoveredDevice() = default;

   //--------------------------------------------------------------
   /// \brief	    Get the device IP
   /// \return     The device IP
   //--------------------------------------------------------------	
   std::string ip() const;


   //--------------------------------------------------------------
   /// \brief	    Find a specific tag value from SSDP discovered device (in 'device' block)
   /// \param[in]  tagName             The requested tag
   /// \return     Tag value or std::out_of_range exception if not found
   //--------------------------------------------------------------	
	std::string findDeviceTag(const std::string& tagName) const;

private:
   static boost::shared_ptr<shared::CDataContainer> fromXml(const std::string& deviceDescription);

   const boost::shared_ptr<shared::CDataContainer> m_description;
};
