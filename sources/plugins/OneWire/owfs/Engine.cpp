#include "stdafx.h"
#include "Engine.h"
#include "../Configuration.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/Log.h>

#include "../device/Identification.h"

// All devices
#include "../device/4kEepromWithPio.h"
#include "../device/4kRamWithCounter.h"
#include "../device/8ChannelAddressableSwitch.h"
#include "../device/AdressableSwitch.h"
#include "../device/DigitalPotentiometer.h"
#include "../device/DualAddressableSwitchPlus1kMemory.h"
#include "../device/DualChannelAddressableSwitch.h"
#include "../device/EconoDigitalThermometer.hpp"
#include "../device/HighPrecisionLiBatteryMonitor.h"
#include "../device/HighPrecisionDigitalThermometer.hpp"
#include "../device/MicrolanCoupler.h"
#include "../device/ProgrammableResolutionDigitalThermometer.hpp"
#include "../device/QuadAdConverter.h"
#include "../device/SmartBatteryMonitor.h"
#include "../device/TemperatureIo.h"
#include "../device/TemperatureMemory.hpp"
#include "../device/Thermachron.hpp"

// Io access to devices for OWFS
#include "io/MultiCounter.h"
#include "io/DualAddressableSwitchPlus1kMemory.h"
#include "io/DigitalPotentiometer.h"
#include "io/HighPrecisionLiBatteryMonitor.h"
#include "io/MultiSwitch.h"
#include "io/MicrolanCoupler.h"
#include "io/QuadAdConverter.h"
#include "io/SingleSwitch.h"
#include "io/SmartBatteryMonitor.h"
#include "io/Temperature.hpp"
#include "io/TemperatureIo.h"


namespace owfs
{
   static const boost::filesystem::path OwfsBaseDir("/mnt/1wire/uncached");

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

      return m_configuration->getOwfsMountPoint() != newConfiguration.getOwfsMountPoint();
   }


   std::map<std::string, boost::shared_ptr<device::IDevice> > CEngine::scanNetwork()
   {
      std::map<std::string, boost::shared_ptr<device::IDevice> > devices;
      scanNetworkNode(OwfsBaseDir, devices);
      return devices;
   }

   void CEngine::scanNetworkNode(const boost::filesystem::path& nodePath,
                                 std::map<std::string,
                                 boost::shared_ptr<device::IDevice> >& devices)
   {
      try
      {
         if (!boost::filesystem::exists(nodePath))
            return;

         if (!boost::filesystem::is_directory(nodePath))
            return;

         static const boost::filesystem::directory_iterator endDirectoryIterator;
         for (boost::filesystem::directory_iterator dir(nodePath); dir != endDirectoryIterator; ++dir)
         {
            // Check dir
            if (!isValidDir(*dir))
               continue;

            // Get the device from it's path and add it to list
            try
            {
               boost::shared_ptr<device::IDevice> device(createDevice(*dir));
               devices[device->ident()->deviceName()] = device;

               // If device is a hub, scan for all hub connected devices (recursively)
               if (device->ident()->family() == kMicrolanCoupler)
               {
                  // Scan in "main" and "aux" dir
                  scanNetworkNode(nodePath / boost::filesystem::path("main"), devices);
                  scanNetworkNode(nodePath / boost::filesystem::path("aux"), devices);
               }
            }
            catch (shared::exception::CInvalidParameter&)
            {
               const std::string unsupportedFamily(dir->path().filename().string().substr(0, 2));
               if (m_unsupporterFamilies.find(unsupportedFamily) == m_unsupporterFamilies.end())
               {
                  YADOMS_LOG(information) << "1-Wire, Device family 0x" << unsupportedFamily << " is not actually supported";
                  m_unsupporterFamilies.insert(unsupportedFamily);
               }
            }
         }
      }
      catch (const boost::filesystem::filesystem_error& ex)
      {
         YADOMS_LOG(error) << "1-Wire, OWFS mode, unable to scan 1-wire network : " << ex.what();
      }
   }

   bool CEngine::isValidDir(const boost::filesystem::path& path) const
   {
      // Check dirent type
      if (!boost::filesystem::is_directory(path))
         return false;

      // Check dir name format (must be something like xx.xxxxxxxxxxxx where x is hexa)
      if (!boost::regex_match(path.filename().string(), boost::regex("[[:xdigit:]]{2}.[[:xdigit:]]{12}")))
         return false;

      // The dir is a valid device name
      return true;
   }

   boost::shared_ptr<device::IDevice> CEngine::createDevice(const boost::filesystem::path& devicePath) const
   {
      const std::string filename = devicePath.filename().string();

      // OWFS device name format, see http://owfs.org/uploads/owfs.1.html#sect30
      // OWFS must be configured to use the default format ff.iiiiiiiiiiii, with :
      // - ff : family (1 byte)
      // - iiiiiiiiiiii : id (6 bytes)

      // Retrieve family from the first 2 chars
      EOneWireFamily family = ToFamily(filename.substr(0, 2));

      // Device Id (6 chars after '.')
      std::string id = filename.substr(3, 6 * 2);
      // OWFS give us the device ID inverted, so reinvert it
      for (int i = 0; i < 6 / 2; i++)
      {
         int left_position = i * 2;          // Point on first byte
         int right_position = (6 - i - 1) * 2;   // Point on last byte
         char c = id[left_position]; id[left_position] = id[right_position]; id[right_position] = c;
         c = id[left_position + 1]; id[left_position + 1] = id[right_position + 1]; id[right_position + 1] = c;
      }

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
         device = boost::make_shared<device::CHighPrecisionDigitalThermometer>(family, id, boost::make_shared<io::CTemperature>(devicePath));
         break;

      case kThermachron:
         device = boost::make_shared<device::CThermachron>(family, id, boost::make_shared<io::CTemperature>(devicePath));
         break;

      case kEconoDigitalThermometer:
         device = boost::make_shared<device::CEconoDigitalThermometer>(family, id, boost::make_shared<io::CTemperature>(devicePath));
         break;

      case kTemperatureMemory:
         device = boost::make_shared<device::CTemperatureMemory>(family, id, boost::make_shared<io::CTemperature>(devicePath));
         break;

      case kProgrammableResolutionDigitalThermometer:
         device = boost::make_shared<device::CProgrammableResolutionDigitalThermometer>(family, id, boost::make_shared<io::CTemperature>(devicePath));
         break;

      case kAddresableSwitch:
         device = boost::make_shared<device::CAdressableSwitch>(family, id, boost::make_shared<io::CSingleSwitch>(devicePath));
         break;

      case kMicrolanCoupler:
         device = boost::make_shared<device::CMicrolanCoupler>(family, id, boost::make_shared<io::CMicrolanCoupler>(devicePath));
         break;

      case kDualAddressableSwitchPlus1kMemory:
         device = boost::make_shared<device::CDualAddressableSwitchPlus1kMemory>(family, id, boost::make_shared<io::CDualAddressableSwitchPlus1kMemory>(devicePath));
         break;

      case k8ChannelAddressableSwitch:
         device = boost::make_shared<device::C8ChannelAddressableSwitch>(family, id, boost::make_shared<io::CMultiSwitch>(devicePath, io::CMultiSwitch::kUnitIsZeroBasedNumber));
         break;

      case kDigitalPotentiometer:
         device = boost::make_shared<device::CDigitalPotentiometer>(family, id, boost::make_shared<io::CDigitalPotentiometer>(devicePath));
         break;

      case kTemperatureIo:
         device = boost::make_shared<device::CTemperatureIo>(family, id, boost::make_shared<io::CTemperatureIo>(devicePath));
         break;

      case kDualChannelAddressableSwitch:
         device = boost::make_shared<device::CDualChannelAddressableSwitch>(family, id, boost::make_shared<io::CMultiSwitch>(devicePath, io::CMultiSwitch::kUnitIsUppercaseLetter));
         break;

      case k4kEepromWithPio:
         device = boost::make_shared<device::C4kEepromWithPio>(family, id, boost::make_shared<io::CMultiSwitch>(devicePath, io::CMultiSwitch::kUnitIsZeroBasedNumber));
         break;

      case k4kRamWithCounter:
         device = boost::make_shared<device::C4kRamWithCounter>(family, id, boost::make_shared<io::CMultiCounter>(devicePath));
         break;

      case kQuadAdConverter:
         device = boost::make_shared<device::CQuadAdConverter>(family, id, boost::make_shared<io::CQuadAdConverter>(devicePath));
         break;

      case kSmartBatteryMonitor:
         device = boost::make_shared<device::CSmartBatteryMonitor>(family, id, boost::make_shared<io::CSmartBatteryMonitor>(devicePath));
         break;

      case kHighPrecisionLiBatteryMonitor:
         device = boost::make_shared<device::CHighPrecisionLiBatteryMonitor>(family, id, boost::make_shared<io::CHighPrecisionLiBatteryMonitor>(devicePath));
         break;

      default: // Device is not actually supported
      {
         throw shared::exception::CInvalidParameter("1-wire device family");
      }
      }

      return device;
   }
} // namespace owfs
