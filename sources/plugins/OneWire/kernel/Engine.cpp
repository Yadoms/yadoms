#include "stdafx.h"
#include "Engine.h"
#include "../Configuration.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/Log.h>

#include "../device/Identification.h"

// All devices
#include "../device/8ChannelAddressableSwitch.h"
#include "../device/DualChannelAddressableSwitch.h"
#include "../device/HighPrecisionDigitalThermometer.hpp"

// Io access to devices for kernel mode
#include "io/8ChannelAddressableSwitch.h"
#include "io/DualChannelAddressableSwitch.h"
#include "io/HighPrecisionDigitalThermometer.h"


namespace kernel
{
   CEngine::CEngine(boost::shared_ptr<yApi::IYPluginApi> api,
                    boost::shared_ptr<const IConfiguration> configuration)
      :m_api(api),
      m_configuration(configuration)
   {
   }

   CEngine::~CEngine()
   {
   }

   bool CEngine::newConfigurationRequireRestart(const shared::CDataContainer& newConfigurationData) const
   {
      CConfiguration newConfiguration;
      newConfiguration.initializeWith(newConfigurationData);

      return m_configuration->getKernelMountPoint() != newConfiguration.getKernelMountPoint();
   }


   std::map<std::string, boost::shared_ptr<device::IDevice> > CEngine::scanNetwork()
   {
      std::map<std::string, boost::shared_ptr<device::IDevice> > devices;

      boost::filesystem::path slavesFile = m_configuration->getKernelMountPoint() / boost::filesystem::path("w1_bus_master1/w1_master_slaves");

      scanNetworkNode(slavesFile, devices);
      return devices;
   }

   void CEngine::scanNetworkNode(const boost::filesystem::path& slavesFile,
                                 std::map<std::string, boost::shared_ptr<device::IDevice> >& devices)
   {
      try
      {
         if (!boost::filesystem::exists(slavesFile))
            return;

         std::string line;
         std::ifstream infile(slavesFile.string());
         if (!infile.is_open())
         {
            YADOMS_LOG(information) << "1-Wire, Unable to read devices from " << slavesFile;
            return;
         }

         while (getline(infile, line))
         {
            boost::shared_ptr<device::IDevice> device(createDevice(line));
            devices[device->ident()->deviceName()] = device;
         }
      }
      catch (...)
      {
         YADOMS_LOG(error) << "1-Wire, OWFS mode, unable to scan 1-wire network : ";
      }
   }

   boost::shared_ptr<device::IDevice> CEngine::createDevice(const std::string& line) const
   {
      // 1W-Kernel device name format : ff-iiiiiiiiiiii, with :
      // - ff : family (1 byte)
      // - iiiiiiiiiiii : id (6 bytes)

      // Retrieve family from the first 2 chars
      EOneWireFamily family = ToFamily(line.substr(0, 2));

      // Device Id (6 chars after '.')
      std::string id = line.substr(3, 6 * 2);
      boost::to_upper(id);

      // Device path
      boost::filesystem::path devicePath = m_configuration->getKernelMountPoint() / line;

      return createDevice(family, id, devicePath);
   }

   boost::shared_ptr<device::IDevice> CEngine::createDevice(EOneWireFamily family,
                                                            const std::string& id,
                                                            const boost::filesystem::path& devicePath) const
   {
      boost::shared_ptr<device::IDevice> device;

      switch (family)
      {
      case kHighPrecisionDigitalThermometer:
         device = boost::make_shared<device::CHighPrecisionDigitalThermometer>(family,
                                                                               id,
                                                                               boost::make_shared<io::CHighPrecisionDigitalThermometer>(devicePath));
         break;

      case kDualChannelAddressableSwitch:
         device = boost::make_shared<device::CDualChannelAddressableSwitch>(family,
                                                                            id,
                                                                            boost::make_shared<io::CDualChannelAddressableSwitch>(devicePath));
         break;

      case k8ChannelAddressableSwitch:
         device = boost::make_shared<device::C8ChannelAddressableSwitch>(family,
                                                                         id,
                                                                         boost::make_shared<io::C8ChannelAddressableSwitch>(devicePath));
         break;

      default: // Device is not actually supported
      {
         throw shared::exception::CInvalidParameter("1-wire device family");
      }
      }

      return device;
   }
} // namespace kernel
