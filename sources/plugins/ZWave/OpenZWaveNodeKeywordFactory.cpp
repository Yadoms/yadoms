#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include <shared/exception/NotSupported.hpp>
#include <shared/enumeration/IExtendedEnum.h>
#include "OpenZWaveCommandClass.h"


#include <value_classes/ValueBool.h>
#include <value_classes/ValueDecimal.h>
#include <value_classes/ValueList.h>
#include <Manager.h>

#include <shared/Log.h>
#include "OpenZWaveNodeKeywordBase.h"
#include "OpenZWaveNodeKeywordGeneric.h"
#include "OpenZWaveNodeKeywordDimmable.h"
#include "OpenZWaveEnumHandlerTypeInfo.h"

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

#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include "OpenZWaveHelpers.h"

std::vector< boost::shared_ptr<shared::plugin::yPluginApi::CStandardCapacity> > COpenZWaveNodeKeywordFactory::m_declaredCapacities;


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(OpenZWave::ValueID & vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   return generateHistoriser(vID, homeId, nodeId, includeSystemKeywords);
}


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateHistoriser(OpenZWave::ValueID & vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   try
   {
      if (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_User || ( (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_System || vID.GetGenre() == OpenZWave::ValueID::ValueGenre_Config)&& includeSystemKeywords))
      {
         ECommandClass commandClass((int)vID.GetCommandClassId());
         std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
         uint8 commandClassInt = vID.GetCommandClassId();
         bool readOnly = OpenZWave::Manager::Get()->IsValueReadOnly(vID);
         bool writeOnly = OpenZWave::Manager::Get()->IsValueWriteOnly(vID);
         bool polled = OpenZWave::Manager::Get()->IsValuePolled(vID);
         bool valueSet = OpenZWave::Manager::Get()->IsValueSet(vID);
         std::string units = OpenZWave::Manager::Get()->GetValueUnits(vID);
         bool awake = OpenZWave::Manager::Get()->IsNodeAwake(homeId, nodeId);
         bool failed = OpenZWave::Manager::Get()->IsNodeFailed(homeId, nodeId);
         bool zwavePlus = OpenZWave::Manager::Get()->IsNodeZWavePlus(homeId, nodeId);
         uint8 instance = vID.GetInstance();

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

         YADOMS_LOG(debug) << "[" << homeId << "." << nodeId << "] " << vLabel << ":" << commandClass.toString()
            << " [Genre=" << genre << "]"
            << " [CommandClass=" << (int)commandClassInt << "]"
            << " [Instance=" << (int)instance << "]"
            << " [ReadOnly=" << readOnly << "]"
            << " [WriteOnly= " << writeOnly << "]"
            << " [Polled= " << polled << "]"
            << " [Set= " << valueSet << "]"
            << " [Units= " << units << "]"
            << " [Awake= " << awake << "]"
            << " [Failed= " << failed << "]"
            << " [ZWave+= " << zwavePlus << "]";

         shared::plugin::yPluginApi::EKeywordAccessMode accessMode = shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet;
         if (readOnly)
            accessMode = shared::plugin::yPluginApi::EKeywordAccessMode::kGet;

         boost::shared_ptr<IOpenZWaveNodeKeyword> genericKeyword = generateStandardKeyword(vLabel, vID, accessMode, commandClass);
         if (genericKeyword)
            return genericKeyword;
         return generateSpecificKeyword(vLabel, units, vID, accessMode);
      }
   }
   catch (OpenZWave::OZWException & ex)
   {
      YADOMS_LOG(error) << "OpenZWave::OZWException : "  << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Unknown exception";
   }

   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateStandardKeyword(const std::string & vLabel, OpenZWave::ValueID & vID, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, const ECommandClass & commandClass)
{
   //try to get already known keyword/capacity
   switch (commandClass)
   {
   case ECommandClass::kEnergyProductionValue:
      if (vLabel == "Instant energy production")
      {
         boost::shared_ptr<historizers::CEnergy> historizer(new historizers::CEnergy(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Total energy production" || vLabel == "Energy production today")
      {
         boost::shared_ptr<historizers::CEnergy> historizer(new historizers::CEnergy(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kCumulative));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      break;
   case ECommandClass::kMeterValue:
      if (vLabel == "Energy")
      {
         boost::shared_ptr<historizers::CEnergy> historizer(new historizers::CEnergy(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Power")
      {
         boost::shared_ptr<historizers::CPower> historizer(new historizers::CPower(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Count")
      {
         boost::shared_ptr<historizers::CCounter> historizer(new historizers::CCounter(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<Poco::Int64>::create(historizer, vID);
      }
      if (vLabel == "Voltage")
      {
         boost::shared_ptr<historizers::CVoltage> historizer(new historizers::CVoltage(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Current")
      {
         boost::shared_ptr<historizers::CCurrent> historizer(new historizers::CCurrent(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Power Factor")
      {
         boost::shared_ptr<historizers::CPowerFactor> historizer(new historizers::CPowerFactor(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      break;

   case ECommandClass::kMeterPulseValue:
      if (vLabel == "Count")
      {
         boost::shared_ptr<historizers::CCounter> historizer(new historizers::CCounter(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<Poco::Int64>::create(historizer, vID);
      }
      break;

   case ECommandClass::kSensorMultilevelValue:

      if (vLabel == "Temperature" || vLabel == "Water Temperature" || vLabel == "Soil Temperature")
      {
         boost::shared_ptr<historizers::CTemperature> historizer(new historizers::CTemperature(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Luminance")
      {
         if (OpenZWave::Manager::Get()->GetValueUnits(vID) == "lux")
         {
            boost::shared_ptr<historizers::CIllumination> historizer(new historizers::CIllumination(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
            return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
         }
      }

      if (vLabel == "Power")
      {
         boost::shared_ptr<historizers::CPower> historizer(new historizers::CPower(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Relative Humidity")
      {
         boost::shared_ptr<historizers::CHumidity> historizer(new historizers::CHumidity(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Velocity")
      {
         boost::shared_ptr<historizers::CSpeed> historizer(new historizers::CSpeed(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Direction")
      {
         boost::shared_ptr<historizers::CDirection> historizer(new historizers::CDirection(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<Poco::Int32>::create(historizer, vID);
      }

      if (vLabel == "Barometric Pressure")
      {
         boost::shared_ptr<historizers::CPressure> historizer(new historizers::CPressure(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Rain Rate")
      {
         boost::shared_ptr<historizers::CRainRate> historizer(new historizers::CRainRate(COpenZWaveHelpers::GenerateKeywordName(vID)));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Weight")
      {
         boost::shared_ptr<historizers::CWeight> historizer(new historizers::CWeight(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Voltage")
      {
         boost::shared_ptr<historizers::CVoltage> historizer(new historizers::CVoltage(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      
      if (vLabel == "Current")
      {
         boost::shared_ptr<historizers::CCurrent> historizer(new historizers::CCurrent(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Distance")
      {
         boost::shared_ptr<historizers::CDistance> historizer(new historizers::CDistance(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Ultraviolet")
      {
         boost::shared_ptr<historizers::CUv> historizer(new historizers::CUv(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Moisture")
      {
         if (OpenZWave::Manager::Get()->GetValueUnits(vID) == "%")
         {
            boost::shared_ptr<historizers::CHumidity> historizer(new historizers::CHumidity(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
            return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
         }
      }

      break;

   case ECommandClass::kSwitchBinaryValue:
      if (vLabel == "Switch")
      {
         boost::shared_ptr<historizers::CSwitch> historizer(new historizers::CSwitch(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<bool>::create(historizer, vID);
      }
      break;
   case ECommandClass::kSwitchMultilevelValue:
      if (vLabel == "Level")
      {
         return boost::shared_ptr<COpenZWaveNodeKeywordDimmable>(new COpenZWaveNodeKeywordDimmable(vID, vLabel, accessMode));
      }
      break;
   case ECommandClass::kSwitchToggleBinaryValue:
      if (vLabel == "Toggle Switch")
      {
         boost::shared_ptr<historizers::CSwitch> historizer(new historizers::CSwitch(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<bool>::create(historizer, vID);
      }
      break;
   case ECommandClass::kSwitchToggleMultilevelValue:
      if (vLabel == "Level")
      {
         return boost::shared_ptr<COpenZWaveNodeKeywordDimmable>(new COpenZWaveNodeKeywordDimmable(vID, vLabel, accessMode));
      }
      break;
   }

   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}



boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateSpecificKeyword(const std::string & vLabel, const std::string & units, OpenZWave::ValueID & vID, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
{
   //this an unknown capacity
   //try to create it depending on the valueType
   switch (vID.GetType())
   {
      case  OpenZWave::ValueID::ValueType_Bool:			// Boolean, true or false
      {
         return COpenZWaveNodeKeywordGeneric<bool>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kBool);
      }

      case  OpenZWave::ValueID::ValueType_Byte:			// 8-bit unsigned value
      {
         return COpenZWaveNodeKeywordGeneric<Poco::UInt8>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric);
      }

      case  OpenZWave::ValueID::ValueType_Decimal:		// Represents a non-integer value as a string, to avoid floating point accuracy issues
      {
         return COpenZWaveNodeKeywordGeneric<double>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric);
      }

      case  OpenZWave::ValueID::ValueType_Int:			// 32-bit signed value
      {
         return COpenZWaveNodeKeywordGeneric<Poco::Int32>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric);
      }

      case  OpenZWave::ValueID::ValueType_List:			// List from which one item can be selected
      {
         COpenZWaveEnumHandlerTypeInfo ti(vID);
         return COpenZWaveNodeKeywordGeneric< COpenZWaveEnumHandler >::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kEnum, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute, ti);
      }

      case  OpenZWave::ValueID::ValueType_Schedule:			// Complex type used with the Climate Control Schedule command class
      {
         YADOMS_LOG(information) << "Value Schedule : ";
         break;
      }

      case  OpenZWave::ValueID::ValueType_Short:			// 16-bit signed value
      {
         return COpenZWaveNodeKeywordGeneric<Poco::Int16>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric);
      }

      case  OpenZWave::ValueID::ValueType_String:			// Text string
      {
         return COpenZWaveNodeKeywordGeneric<std::string>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kString);
      }

      case  OpenZWave::ValueID::ValueType_Button:			// A write-only value that is the equivalent of pressing a button to send a command to a device
      {
         return COpenZWaveNodeKeywordGeneric<bool>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kBool);
      }

      case  OpenZWave::ValueID::ValueType_Raw:			// A collection of bytes
      {
         return COpenZWaveNodeKeywordGeneric<std::string>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kString);
      }
   }
   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}

const shared::plugin::yPluginApi::CStandardCapacity& COpenZWaveNodeKeywordFactory::getCapacity(const std::string & name, const std::string & unit, shared::plugin::yPluginApi::EKeywordDataType dataType)
{
   m_declaredCapacities.push_back(boost::shared_ptr<shared::plugin::yPluginApi::CStandardCapacity>(new shared::plugin::yPluginApi::CStandardCapacity(name, unit, dataType)));
   return *(m_declaredCapacities.back().get());
}