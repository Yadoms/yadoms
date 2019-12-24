#include "stdafx.h"
#include "Device.h"
#include "shared/DataContainer.h"

namespace hardware
{
   namespace usb
   {
      CDevice::CDevice(const LibUSB::Device& libusbppDevice)
         :m_libusbppDevice(libusbppDevice)
      {   
      }

      std::string CDevice::yadomsConnectionId() const
      {
         return std::to_string(vendorId()) + ";" + std::to_string(productId()) + ";" + serialNumber();
      }

      int CDevice::vendorId() const
      {
         //TODO
         return 0x0fd9;
      }

      int CDevice::productId() const
      {
         //TODO
         return 0x0060;
      }

      std::string CDevice::serialNumber() const
      {
         //TODO
         return "AL25I1C03149";
      }

      std::string CDevice::yadomsFriendlyName() const
      {
         //TODO
         return "Stream Deck XL - CL30I1A03593";
      }

      shared::CDataContainer CDevice::toContainer() const
      {
         //TODO
         shared::CDataContainer todo;
         todo.set("Product", "Stream Deck XL");
         todo.set("Manufacturer", "Elgato");
         todo.set("SerialNumber", "CL30I1A03593");
         return todo;
      }
   } // namespace usb
} // namespace hardware
