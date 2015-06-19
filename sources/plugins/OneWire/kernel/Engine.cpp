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


namespace kernel {

//TODO remettre static const boost::filesystem::path baseDir("/sys/bus/w1/devices");
static const boost::filesystem::path baseDir("~/sys/bus/w1/devices"); //TODO utiliser la conf


CEngine::CEngine(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const IConfiguration> configuration)
   :m_context(context), m_configuration(configuration)
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


std::map<std::string, boost::shared_ptr<device::IDevice> > CEngine::scanNetwork() const
{
   std::map<std::string, boost::shared_ptr<device::IDevice> > devices;

   boost::filesystem::path slavesFile = baseDir / boost::filesystem::path("w1_bus_master1/w1_master_slaves");

   scanNetworkNode(slavesFile, devices);
   return devices;
}

void CEngine::scanNetworkNode(const boost::filesystem::path& slavesFile, std::map<std::string, boost::shared_ptr<device::IDevice> >& devices) const
{
   try
   {
      if (!boost::filesystem::exists(slavesFile))
         return;

      std::string line;
      std::ifstream infile(slavesFile.string());
      if (!infile.is_open())
      {
         YADOMS_LOG(warning) << "1-Wire, Unable to read devices from " << slavesFile;
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
   boost::filesystem::path devicePath = baseDir / line;

   return createDevice(family, id, devicePath);
}

boost::shared_ptr<device::IDevice> CEngine::createDevice(EOneWireFamily family, const std::string& id, const boost::filesystem::path& devicePath) const
{
   boost::shared_ptr<device::IDevice> device;

   //TODO à virer
   //switch (family)
   //{
   //case high_precision_digital_thermometer:
   //case dual_channel_addressable_switch:
   //case _8_channel_addressable_switch:
   //case programmable_resolution_digital_thermometer:
   //   m_Devices[device.devid] = new DeviceState(device);
   //   _log.Log(LOG_STATUS, "1Wire: Added Device: %s", sLine.c_str());
   //   break;
   //default: // Device not supported in kernel mode (maybe later...), use OWFS solution.
   //   _log.Log(LOG_ERROR, "1Wire: Device not yet supported in Kernel mode (Please report!) ID:%s, family: %02X", sLine.c_str(), device.family);
   //   break;
   //}

   switch (family)
   {
   case kHighPrecisionDigitalThermometer:
      device = boost::make_shared<device::CHighPrecisionDigitalThermometer>(family, id, m_context, boost::make_shared<io::CHighPrecisionDigitalThermometer>(devicePath));
      break;

   case kDualChannelAddressableSwitch:
      device = boost::make_shared<device::CDualChannelAddressableSwitch>(family, id, m_context, boost::make_shared<io::CDualChannelAddressableSwitch>(devicePath));
      break;

   case k8ChannelAddressableSwitch:
      device = boost::make_shared<device::C8ChannelAddressableSwitch>(family, id, m_context, boost::make_shared<io::C8ChannelAddressableSwitch>(devicePath));
      break;

   default: // Device is not actually supported
   {
      throw shared::exception::CInvalidParameter("1-wire device family");
   }
   }

   return device;
}

} // namespace kernel
