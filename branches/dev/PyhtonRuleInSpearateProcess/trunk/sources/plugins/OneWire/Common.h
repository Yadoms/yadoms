#pragma once

// 1-wire chip family definition (merge from http://owfs.sourceforge.net/family.html and http://owfs.org/index.php?page=family-code-list)
enum EOneWireFamily
{
   kSiliconSerialNumber	= 0x01,
   kMultikey1153bitSecure	= 0x02,
   kEconoramTimeChip	= 0x04,
   kAddresableSwitch	= 0x05,
   k4kMemoryIbutton	= 0x06,
   k1kMemoryIbutton	= 0x08,
   k1kAddOnlyMemory	= 0x09,
   k16kMemoryIbutton	 = 0x0A,
   k16kAddOnlyMemory	 = 0x0B,
   k64kMemoryIbutton	 = 0x0C,
   k64kAddOnlyMemory	 = 0x0F,
   kHighPrecisionDigitalThermometer	= 0x10,
   kDualAddressableSwitchPlus1kMemory	= 0x12,
   k256Eeprom	= 0x14,
   kCryptoIbutton	= 0x16,
   kSHAIButton	= 0x18,
   k4kMonetary	 = 0x1A,
   kBatteryIdMonitorChip	 = 0x1B,
   k4kEepromWithPio	 = 0x1C,
   k4kRamWithCounter	 = 0x1D,
   kBatteryMonitor	= 0x1E,
   kMicrolanCoupler	 = 0x1F,
   kQuadAdConverter	= 0x20,
   kThermachron	= 0x21,
   kEconoDigitalThermometer	= 0x22,
   k4kEeprom	= 0x23,
   kTimeChip	= 0x24,
   kSmartBatteryMonitor	= 0x26,
   kTimeChipWithInterrupt	= 0x27,
   kProgrammableResolutionDigitalThermometer	= 0x28,
   k8ChannelAddressableSwitch	= 0x29,
   kDigitalPotentiometer	 = 0x2C,
   k1kEeprom	 = 0x2D,
   kBatteryMonitorAndChargeController	 = 0x2E,
   kHighPrecisionLiBatteryMonitor	= 0x30,
   kEfficientAddressableSingleCellRechargableLithiumProtectionIc	= 0x31,
   kBattery = 0x32,
   k1kProtectedEepromWithSha1	= 0x33,
   kSHA1Battery = 0x34,
   kBatteryFuelGauge = 0x35,
   kHighPrecisionCoulombCounter	= 0x36,
   kPasswordProtected32kEeprom	= 0x37,
   kDualChannelAddressableSwitch	 = 0x3A,
   kTemperatureMemory = 0x3B,
   kStandAloneFuelGauge = 0x3D,
   kTemperatureLogger8kMem	= 0x41,
   kTemperatureIo	= 0x42,
   kMemory = 0x43,
   kSha1Authenticator = 0x44,
   kMultichemistryBatteryFuelGauge	= 0x51,
   kEnvironmentalMonitors = 0x7E,
   kSerialIdButton	= 0x81,
   kAuthorization = 0x82,
   kDualPortPlusTime	= 0x84,
   k48BitNodeAddressChip	= 0x89,
   k16kAddOnlyUniqueware	 = 0x8B,
   k64kAddOnlyUniqueware	 = 0x8F,
   kRotationSensor = 0xA0,
   kVibratio	= 0xA1,
   kAcVoltage	= 0xA2,
   kIrTemperature	= 0xA6,
   k21ChannelsThermocoupleConverter	= 0xB1,
   kDcCurrentOrVoltage	= 0xB2,
   k168ChannelsThermocoupleConverter	= 0xB3,
   kUltraVioletIndex	= 0xEE,
   kMoistureMeter4ChannelHub	= 0xEF,
   kProgrammableMiroprocessor	= 0xFC,
   kLcdSwart	 = 0xFF,

   kUnknown = 0
};


//-----------------------------------------------------
///\brief                     Convert family number as hex string to Family enum value
///\param[in] str             Family number as hex string
///\return                    Family enum value
//-----------------------------------------------------
EOneWireFamily ToFamily(const std::string& str);

//-----------------------------------------------------
///\brief                     Compute 1-wire CRC on a byte array
///\param[in] byteArray       Pointer on byte array
///\param[in] arraySize       Size of array
///\return                    Computed CRC
//-----------------------------------------------------
unsigned char Crc16(const unsigned char* byteArray, size_t arraySize);
