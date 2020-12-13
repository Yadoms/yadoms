#pragma once
#include "../StreamDeckEnum.h"

class CUsbDeviceInformation
{
public:
   CUsbDeviceInformation() = default;
  
   uint16_t getVendorId() const;
   void setVendorId(uint16_t vendorId);
   uint16_t getProductId() const;
   void setProductId(uint16_t productId);
   const std::string& getSerialNumber() const;
   void setSerialNumber(const std::string& serialNumber);
   const std::string& getDeviceName() const;
   void setDeviceName(const std::string& deviceName);
   const std::string& getDeviceType() const;
   void setDeviceType(const std::string& deviceType);
   int getKeyCols() const;
   void setKeyCols(int keyCols);
   int getKeyRows() const;
   void setKeyRows(int keyRows);
   int getKeyCount() const;
   void setKeyCount(int keyCount);
   CStreamDeckEnum::EStreamDeckType getDeviceModel() const;
   void setDeviceModel(CStreamDeckEnum::EStreamDeckType deviceModel);

private:
	uint16_t m_vendorId;
	uint16_t m_productId;
	std::string m_serialNumber;
	std::string m_deviceName = "Stream Deck";
	std::string m_deviceType;
	int m_keyCols;
	int m_keyRows;
	int m_keyCount;
   CStreamDeckEnum::EStreamDeckType m_deviceModel;
};
