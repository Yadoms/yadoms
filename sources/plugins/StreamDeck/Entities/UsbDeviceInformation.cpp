#include "UsbDeviceInformation.h"

uint16_t CUsbDeviceInformation::getVendorId() const
{
   return m_vendorId;
}

void CUsbDeviceInformation::setVendorId(uint16_t vendorId)
{
   m_vendorId = vendorId;
}

uint16_t CUsbDeviceInformation::getProductId() const
{
   return m_productId;
}

void CUsbDeviceInformation::setProductId(uint16_t productId)
{
   m_productId = productId;
}

const std::string& CUsbDeviceInformation::getSerialNumber() const
{
   return m_serialNumber;
}

void CUsbDeviceInformation::setSerialNumber(const std::string& serialNumber)
{
   m_serialNumber = serialNumber;
}

const std::string& CUsbDeviceInformation::getDeviceName() const
{
   return m_deviceName;
}

void CUsbDeviceInformation::setDeviceName(const std::string& deviceName)
{
   m_deviceName = deviceName;
}

const std::string& CUsbDeviceInformation::getDeviceType() const
{
   return m_deviceType;
}

void CUsbDeviceInformation::setDeviceType(const std::string& deviceType)
{
   m_deviceType = deviceType;
}

int CUsbDeviceInformation::getKeyCols() const
{
   return m_keyCols;
}

void CUsbDeviceInformation::setKeyCols(int keyCols)
{
   m_keyCols = keyCols;
}

int CUsbDeviceInformation::getKeyRows() const
{
   return m_keyRows;
}

void CUsbDeviceInformation::setKeyRows(int keyRows)
{
   m_keyRows = keyRows;
}

int CUsbDeviceInformation::getKeyCount() const
{
   return m_keyCount;
}

void CUsbDeviceInformation::setKeyCount(int keyCount)
{
   m_keyCount = keyCount;
}

CStreamDeckEnum::EStreamDeckType CUsbDeviceInformation::getDeviceModel() const
{
   return m_deviceModel;
}

void CUsbDeviceInformation::setDeviceModel(CStreamDeckEnum::EStreamDeckType deviceModel)
{
   m_deviceModel = deviceModel;
}
