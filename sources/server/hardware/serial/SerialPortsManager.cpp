#include "stdafx.h"
#include "SerialPortsManager.h"

namespace hardware
{
   namespace serial
   {
      CSerialPortsManager::CSerialPortsManager(boost::shared_ptr<ISerialPortsLister> serialPortLister,
                                               boost::shared_ptr<usb::IDevicesLister> usbDeviceListers)
         : m_serialPortLister(serialPortLister),
           m_usbDeviceListers(usbDeviceListers)
      {
      }

      std::vector<boost::shared_ptr<database::entities::CSerialPort>> CSerialPortsManager::listSerialPorts()
      {
         //TODO éventuellement ajouter un filtre si déjà appelé récemment

         auto serialPorts = m_serialPortLister->listSerialPorts();
         updateAdapterKinds(serialPorts);

         //TODO gérer la maj de la base de données

         return serialPorts;
      }

      void CSerialPortsManager::updateAdapterKinds(
         std::vector<boost::shared_ptr<database::entities::CSerialPort>>& detectedSerialPorts) const
      {
         const auto usbAdapterSerialPorts = m_usbDeviceListers->listUsbDevicesForClass(
            usb::IDevicesLister::kSerialPorts);
         for (auto& detectedSerialPort : detectedSerialPorts)
         {
            const auto correspondingUsbAdapter = std::find_if(
               usbAdapterSerialPorts.begin(),
               usbAdapterSerialPorts.end(),
               [&detectedSerialPort](const auto& usbAdapterSerialPort)
               {
                  return detectedSerialPort->LastKnownConnectionPath == usbAdapterSerialPort->nativeConnectionString();
               });

            detectedSerialPort->AdapterKind = correspondingUsbAdapter != usbAdapterSerialPorts.end()
                                                 ? database::entities::ESerialPortAdapterKind::kUsbAdapter
                                                 : database::entities::ESerialPortAdapterKind::kPhysical;
         }
      }
   } // namespace serial
} // namespace hardware
