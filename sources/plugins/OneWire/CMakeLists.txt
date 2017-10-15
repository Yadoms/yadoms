# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

# This plugin is linux-only
if (UNIX AND NOT APPLE)

   #########################################
   ## Place here all the plugin source files
   #########################################
   set(ONEWIRE_PLUGIN_SOURCES
      stdafx.h
      stdafx.cpp
      Factory.h
      Factory.cpp
      IEngine.h
      IConfiguration.h
      Common.h
      Common.cpp
      Configuration.h
      Configuration.cpp
      OneWire.h
      OneWire.cpp
      OneWireException.hpp
      
      device/4kEepromWithPio.h
      device/4kEepromWithPio.cpp
      device/4kRamWithCounter.h
      device/4kRamWithCounter.cpp
      device/8ChannelAddressableSwitch.h
      device/8ChannelAddressableSwitch.cpp
      device/AdressableSwitch.h
      device/AdressableSwitch.cpp
      device/DigitalPotentiometer.h
      device/DigitalPotentiometer.cpp
      device/DualAddressableSwitchPlus1kMemory.h
      device/DualAddressableSwitchPlus1kMemory.cpp
      device/DualChannelAddressableSwitch.h
      device/DualChannelAddressableSwitch.cpp
      device/EconoDigitalThermometer.hpp
      device/IDevice.h
      device/IIdentification.h
      device/Identification.h
      device/Identification.cpp
      device/HighPrecisionLiBatteryMonitor.h
      device/HighPrecisionLiBatteryMonitor.cpp
      device/HighPrecisionDigitalThermometer.hpp
      device/MicrolanCoupler.h
      device/MicrolanCoupler.cpp
      device/ProgrammableResolutionDigitalThermometer.hpp
      device/QuadAdConverter.h
      device/QuadAdConverter.cpp
      device/SingleTemperature.h
      device/SingleTemperature.cpp
      device/SmartBatteryMonitor.h
      device/SmartBatteryMonitor.cpp
      device/SmartBatteryMonitorConfiguration.h
      device/SmartBatteryMonitorConfiguration.cpp
      device/TemperatureIo.h
      device/TemperatureIo.cpp
      device/TemperatureMemory.hpp
      device/Thermachron.hpp
     
      ioInterfaces/IDigitalPotentiometer.h
      ioInterfaces/IDualAddressableSwitchPlus1kMemory.h
      ioInterfaces/IHighPrecisionLiBatteryMonitor.h
      ioInterfaces/IHumidity.h
      ioInterfaces/IIllumination.h
      ioInterfaces/IMultiCounter.h
      ioInterfaces/IMultiSwitch.h
      ioInterfaces/IPressure.h
      ioInterfaces/IQuadAdConverter.h
      ioInterfaces/ISingleReadOnlyData.h
      ioInterfaces/ISingleSwitch.h
      ioInterfaces/ISmartBatteryMonitor.h
      ioInterfaces/ITemperature.h
      ioInterfaces/ITemperatureIo.h
      ioInterfaces/IVoltage.h
      
      kernel/Engine.h
      kernel/Engine.cpp
      
      kernel/io/Common.h
      kernel/io/Common.cpp
      kernel/io/8ChannelAddressableSwitch.h
      kernel/io/8ChannelAddressableSwitch.cpp
      kernel/io/DualChannelAddressableSwitch.h
      kernel/io/DualChannelAddressableSwitch.cpp
      kernel/io/HighPrecisionDigitalThermometer.h
      kernel/io/HighPrecisionDigitalThermometer.cpp
      
      owfs/Engine.h
      owfs/Engine.cpp
      
      owfs/io/Common.h
      owfs/io/Common.cpp
      owfs/io/DualAddressableSwitchPlus1kMemory.h
      owfs/io/DualAddressableSwitchPlus1kMemory.cpp
      owfs/io/DigitalPotentiometer.h
      owfs/io/DigitalPotentiometer.cpp
      owfs/io/HighPrecisionLiBatteryMonitor.h
      owfs/io/HighPrecisionLiBatteryMonitor.cpp
      owfs/io/Humidity.hpp
      owfs/io/Illumination.hpp
      owfs/io/MultiCounter.h
      owfs/io/MultiCounter.cpp
      owfs/io/MultiSwitch.h
      owfs/io/MultiSwitch.cpp
      owfs/io/MicrolanCoupler.h
      owfs/io/MicrolanCoupler.cpp
      owfs/io/Pressure.hpp
      owfs/io/QuadAdConverter.h
      owfs/io/QuadAdConverter.cpp
      owfs/io/SingleReadOnlyData.hpp
      owfs/io/SingleSwitch.h
      owfs/io/SingleSwitch.cpp
      owfs/io/SmartBatteryMonitor.h
      owfs/io/SmartBatteryMonitor.cpp
      owfs/io/Temperature.hpp
      owfs/io/TemperatureIo.h
      owfs/io/TemperatureIo.cpp
      owfs/io/Voltage.hpp
   )

   #########################################
   ## Place here all additional groups (solution folder)
   #########################################
   source_group(sources ./*.*)
   source_group(sources\\ioInterfaces ioInterfaces/*.*)
   source_group(sources\\device device/*.*)
   source_group(sources\\kernel kernel/*.*)
   source_group(sources\\kernel\\io kernel/io/*.*)
   source_group(sources\\owfs owfs/*.*)
   source_group(sources\\owfs\\io owfs/io/*.*)


   #########################################
   ## Place here all additional include directories
   ## (by default yadoms-shared and boost are included)
   #########################################
      
   set(ONEWIRE_PLUGIN_INCLDIR ${CMAKE_CURRENT_SOURCE_DIR})

   #########################################
   ## Always leave these lines to configure plugin correctly
   ## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
   #########################################
   PLUGIN_SOURCES(OneWire ${ONEWIRE_PLUGIN_SOURCES})
   PLUGIN_INCLDIR(OneWire ${ONEWIRE_PLUGIN_INCLDIR})
   PLUGIN_LINK   (OneWire ${ONEWIRE_PLUGIN_ADDITIONAL_LIB})

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   ## "package.json" and "icon.png" are needed, but you can add extra-files
   #########################################
   PLUGIN_POST_BUILD_COPY_FILE(OneWire package.json)
   PLUGIN_POST_BUILD_COPY_FILE(OneWire icon.png)
   PLUGIN_POST_BUILD_COPY_DIRECTORY(OneWire locales)

endif()
