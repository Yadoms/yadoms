#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include "OpenZWaveCommandClass.h"


#include <Manager.h>

#include "OpenZWaveNodeKeywordBase.h"
#include "OpenZWaveNodeKeywordGeneric.h"
#include "OpenZWaveNodeKeywordDimmable.h"

#include "historizers/Counter.h"
#include "historizers/Current.h"
#include "historizers/Direction.h"
#include "historizers/Distance.h"
#include "historizers/Energy.h"
#include "historizers/Humidity.h"
#include "historizers/Illumination.h"
#include "historizers/Power.h"
#include "historizers/PowerFactor.h"
#include "historizers/Pressure.h"
#include "historizers/RainRate.h"
#include "historizers/Speed.h"
#include "historizers/Switch.h"
#include "historizers/Temperature.h"
#include "historizers/Uv.h"
#include "historizers/Voltage.h"
#include "historizers/Weight.h"

#include "typeinfo/BoolTypeInfo.h"
#include "typeinfo/EnumTypeInfo.h"
#include "typeinfo/DecimalTypeInfo.h"
#include "typeinfo/IntegerTypeInfo.h"
#include "typeinfo/StringTypeInfo.h"

#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include "OpenZWaveHelpers.h"

std::vector<boost::shared_ptr<shared::plugin::yPluginApi::CStandardCapacity> > COpenZWaveNodeKeywordFactory::m_declaredCapacities;


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(OpenZWave::ValueID& vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   return generateHistoriser(vID, homeId, nodeId, includeSystemKeywords);
}


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateHistoriser(OpenZWave::ValueID& vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   try
   {
      if (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_User || vID.GetGenre() == OpenZWave::ValueID::ValueGenre_Config || (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_System && includeSystemKeywords))
      {
         ECommandClass commandClass(static_cast<int>(vID.GetCommandClassId()));
         auto vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
         auto commandClassInt = vID.GetCommandClassId();
         auto readOnly = OpenZWave::Manager::Get()->IsValueReadOnly(vID);
         auto writeOnly = OpenZWave::Manager::Get()->IsValueWriteOnly(vID);
         auto polled = OpenZWave::Manager::Get()->IsValuePolled(vID);
         auto valueSet = OpenZWave::Manager::Get()->IsValueSet(vID);
         auto units = OpenZWave::Manager::Get()->GetValueUnits(vID);
         auto awake = OpenZWave::Manager::Get()->IsNodeAwake(homeId, nodeId);
         auto failed = OpenZWave::Manager::Get()->IsNodeFailed(homeId, nodeId);
         auto zwavePlus = OpenZWave::Manager::Get()->IsNodeZWavePlus(homeId, nodeId);
         auto instance = vID.GetInstance();

         if (units.empty())
            units = shared::plugin::yPluginApi::CStandardUnits::NoUnits;

         std::string genre = "";

         switch (vID.GetGenre())
         {
         case OpenZWave::ValueID::ValueGenre_Basic:
            genre = "Basic";
            break;
         case OpenZWave::ValueID::ValueGenre_Config:
            genre = "Config";
            break;
         case OpenZWave::ValueID::ValueGenre_Count:
            genre = "Count";
            break;
         case OpenZWave::ValueID::ValueGenre_System:
            genre = "System";
            break;
         case OpenZWave::ValueID::ValueGenre_User:
            genre = "User";
            break;
         }

         std::cout << "[" << homeId << "." << nodeId << "] " << vLabel << ":" << commandClass.toString()
            << " [Genre=" << genre << "]"
            << " [CommandClass=" << static_cast<int>(commandClassInt) << "]"
            << " [Instance=" << static_cast<int>(instance) << "]"
            << " [ReadOnly=" << readOnly << "]"
            << " [WriteOnly= " << writeOnly << "]"
            << " [Polled= " << polled << "]"
            << " [Set= " << valueSet << "]"
            << " [Units= " << units << "]"
            << " [Awake= " << awake << "]"
            << " [Failed= " << failed << "]"
            << " [ZWave+= " << zwavePlus << "]" << std::endl;

         shared::plugin::yPluginApi::EKeywordAccessMode accessMode = shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet;
         if (readOnly)
            accessMode = shared::plugin::yPluginApi::EKeywordAccessMode::kGet;

         boost::shared_ptr<IOpenZWaveNodeKeyword> genericKeyword = generateStandardKeyword(vLabel, vID, accessMode, commandClass);
         if (genericKeyword)
            return genericKeyword;
         return generateSpecificKeyword(vLabel, units, vID, accessMode);
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      std::cerr << "OpenZWave::OZWException : " << ex.what() << std::endl;
   }
   catch (...)
   {
      std::cerr << "Unknown exception" << std::endl;
   }

   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateStandardKeyword(const std::string& vLabel, OpenZWave::ValueID& vID, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, const ECommandClass& commandClass)
{
   //try to get already known keyword/capacity
   switch (commandClass)
   {
   case ECommandClass::kEnergyProductionValue:
      if (vLabel == "Instant energy production")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CEnergy>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Total energy production" || vLabel == "Energy production today")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CEnergy>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kCumulative, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      break;
   case ECommandClass::kMeterValue:
      if (vLabel == "Energy")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CEnergy>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Power")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CPower>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Count")
      {
         CIntegerTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CCounter>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<Poco::Int64>::create(historizer, vID);
      }
      if (vLabel == "Voltage")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CVoltage>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Current")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CCurrent>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Power Factor")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CPowerFactor>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      break;

   case ECommandClass::kMeterPulseValue:
      if (vLabel == "Count")
      {
         CIntegerTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CCounter>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<Poco::Int64>::create(historizer, vID);
      }
      break;

   case ECommandClass::kSensorMultilevelValue:

      if (vLabel == "Temperature" || vLabel == "Water Temperature" || vLabel == "Soil Temperature")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CTemperature>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Luminance")
      {
         if (OpenZWave::Manager::Get()->GetValueUnits(vID) == "lux")
         {
            CDecimalTypeInfo ti(vID);
            auto historizer(boost::make_shared<historizers::CIllumination>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
            return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
         }
      }

      if (vLabel == "Power")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CPower>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Relative Humidity")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CHumidity>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Velocity")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CSpeed>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Direction")
      {
         CIntegerTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CDirection>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<Poco::Int32>::create(historizer, vID);
      }

      if (vLabel == "Barometric Pressure")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CPressure>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Rain Rate")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CRainRate>(COpenZWaveHelpers::GenerateKeywordName(vID), ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Weight")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CWeight>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Voltage")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CVoltage>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Current")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CCurrent>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Distance")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CDistance>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Ultraviolet")
      {
         CDecimalTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CUv>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Moisture")
      {
         if (OpenZWave::Manager::Get()->GetValueUnits(vID) == "%")
         {
            CDecimalTypeInfo ti(vID);
            auto historizer(boost::make_shared<historizers::CHumidity>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
            return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
         }
      }

      break;

   case ECommandClass::kSwitchBinaryValue:
      if (vLabel == "Switch")
      {
         CBoolTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CSwitch>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<bool>::create(historizer, vID);
      }
      break;
   case ECommandClass::kSwitchMultilevelValue:
      if (vLabel == "Level")
      {
         CIntegerTypeInfo ti(vID);
         return boost::make_shared<COpenZWaveNodeKeywordDimmable>(vID, vLabel, accessMode, ti);
      }
      break;
   case ECommandClass::kSwitchToggleBinaryValue:
      if (vLabel == "Toggle Switch")
      {
         CBoolTypeInfo ti(vID);
         auto historizer(boost::make_shared<historizers::CSwitch>(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, ti));
         return COpenZWaveNodeKeywordGeneric<bool>::create(historizer, vID);
      }
      break;
   case ECommandClass::kSwitchToggleMultilevelValue:
      if (vLabel == "Level")
      {
         CIntegerTypeInfo ti(vID);
         return boost::make_shared<COpenZWaveNodeKeywordDimmable>(vID, vLabel, accessMode, ti);
      }
      break;
   }

   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateSpecificKeyword(const std::string& vLabel, const std::string& units, OpenZWave::ValueID& vID, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
{
   //this an unknown capacity
   //try to create it depending on the valueType
   switch (vID.GetType())
   {
   case OpenZWave::ValueID::ValueType_Bool: // Boolean, true or false
      {
         CBoolTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<bool>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kBool, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_Byte: // 8-bit unsigned value
      {
         CIntegerTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<Poco::UInt8>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_Decimal: // Represents a non-integer value as a string, to avoid floating point accuracy issues
      {
         CIntegerTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<double>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_Int: // 32-bit signed value
      {
         CIntegerTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<Poco::Int32>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_List: // List from which one item can be selected
      {
         CEnumTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<COpenZWaveEnumHandler>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kEnum, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_Schedule: // Complex type used with the Climate Control Schedule command class
      {
         std::cout << "Value Schedule : " << std::endl;
         break;
      }

   case OpenZWave::ValueID::ValueType_Short: // 16-bit signed value
      {
         CIntegerTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<Poco::Int16>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_String: // Text string
      {
         CStringTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<std::string>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kString, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_Button: // A write-only value that is the equivalent of pressing a button to send a command to a device
      {
         CBoolTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<bool>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kBool, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

   case OpenZWave::ValueID::ValueType_Raw: // A collection of bytes
      {
         CStringTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric<std::string>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kString, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }
   }
   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}

const shared::plugin::yPluginApi::CStandardCapacity& COpenZWaveNodeKeywordFactory::getCapacity(const std::string& name, const std::string& unit, shared::plugin::yPluginApi::EKeywordDataType dataType)
{
   m_declaredCapacities.push_back(boost::make_shared<shared::plugin::yPluginApi::CStandardCapacity>(name, unit, dataType));
   return *(m_declaredCapacities.back().get());
}

