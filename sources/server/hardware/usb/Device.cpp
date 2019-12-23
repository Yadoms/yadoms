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
         //TODO
         return "streamdeckxl";
      }

      std::string CDevice::friendlyName() const
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
