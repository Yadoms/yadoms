#pragma once

#include "../IRule.h"

namespace xplrules { namespace rfxLanXpl {

   class CSensorBasic : public IRule
   {
   public:
      //------------------------------------
      ///\brief Constructor
      //------------------------------------
      CSensorBasic();

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~CSensorBasic();
      
      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      virtual std::vector< boost::shared_ptr<CDeviceKeyword> > identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation

   private:
      //------------------------------------
      ///\brief Check if the device targetted in message is an Oregon one
      ///\param [in] msg the message from which extract the device information
      ///\return true if the device is Oregon
      //------------------------------------
      bool isOregonDevice(xplcore::CXplMessage & msg);

   private:
      static std::string m_keywordHex;
      static std::string m_keywordDevice;
      static std::string m_keywordType;
      static std::string m_keywordTypeBattery;
      static std::string m_keywordTypeDemand;
      static std::string m_keywordTypeVoltage;
      static std::string m_keywordTypeMertik;
      static std::string m_keywordTypeCount;
      static std::string m_keywordTypeHumidity;
      static std::string m_keywordTypeStatus;
      static std::string m_keywordTypePressure;
      static std::string m_keywordTypeDirection;
      static std::string m_keywordTypeUv;
      static std::string m_keywordCurrent;
      static std::string m_keywordDescription;
      static std::string m_keywordForecast;
      static std::string m_keywordUnits;
      static std::string m_keywordTypeDemandValues;
      static std::string m_keywordTypeIoValues;
      static std::string m_keywordTypeMertikValues;
      static std::string m_keywordTypeHumidityValues;
      static std::string m_keywordTypeForecastValues;
      static std::string m_keywordTypeUvDescriptionValues;
      static std::string m_keywordTypeTemp;
      static std::string m_keywordTypeRainRate;
      static std::string m_keywordTypeRainTotal;
      static std::string m_keywordTypeGust;
      static std::string m_keywordTypeAverageSpeed;
      static std::string m_keywordTypeWeight;
      static std::string m_keywordTypePower;
      static std::string m_keywordTypeEnergy;
      static std::string m_keywordDeviceDigimax;
      static std::string m_keywordDeviceRfxSensor;
      static std::string m_keywordDeviceRfxMeter;
      static std::string m_keywordDeviceRfxLanIo;
      static std::string m_keywordDeviceMertik;
      static std::string m_keywordDeviceOregonTemp1;
      static std::string m_keywordDeviceOregonTemp2;
      static std::string m_keywordDeviceOregonTemp3;
      static std::string m_keywordDeviceOregonTemp4;
      static std::string m_keywordDeviceOregonTh1;
      static std::string m_keywordDeviceOregonTh2;
      static std::string m_keywordDeviceOregonTh3;
      static std::string m_keywordDeviceOregonTh4;
      static std::string m_keywordDeviceOregonTh5;
      static std::string m_keywordDeviceOregonTh6;
      static std::string m_keywordDeviceOregonThb1;
      static std::string m_keywordDeviceOregonThb2;
      static std::string m_keywordDeviceOregonRain1;
      static std::string m_keywordDeviceOregonRain2;
      static std::string m_keywordDeviceOregonWind1;
      static std::string m_keywordDeviceOregonWind2;
      static std::string m_keywordDeviceOregonWind3;
      static std::string m_keywordDeviceOregonUv1;
      static std::string m_keywordDeviceOregonUv2;
      static std::string m_keywordDeviceOregonDateTime1;
      static std::string m_keywordDeviceOregonWeight1;
      static std::string m_keywordDeviceOregonWeight2;
      static std::string m_keywordDeviceOregonElec1;
      static std::string m_keywordDeviceOregonElec2;
      static std::string m_keywordDeviceOregonElec3;
      static std::string m_keywordDeviceOregonElec4;
      static std::string m_keywordUnitAmpere;
      static std::string m_keywordUnitVolts;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
