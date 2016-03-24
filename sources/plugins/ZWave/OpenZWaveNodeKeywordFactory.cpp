#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveCommandClass.h"


#include <value_classes/ValueBool.h>
#include <value_classes/ValueDecimal.h>

#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include <shared/plugin/yPluginApi/historization/Temperature.h>
#include <shared/plugin/yPluginApi/historization/Illumination.h>
#include <shared/plugin/yPluginApi/historization/Power.h>
#include <shared/plugin/yPluginApi/historization/Humidity.h>
#include <shared/plugin/yPluginApi/historization/Speed.h>
#include <shared/plugin/yPluginApi/historization/Direction.h>
#include <shared/plugin/yPluginApi/historization/Pressure.h>
#include <shared/plugin/yPluginApi/historization/RainRate.h>
#include <shared/plugin/yPluginApi/historization/Weight.h>
#include <shared/plugin/yPluginApi/historization/Voltage.h>
#include <shared/plugin/yPluginApi/historization/Current.h>
#include <shared/plugin/yPluginApi/historization/Distance.h>
#include <shared/plugin/yPluginApi/historization/Uv.h>
#include <shared/plugin/yPluginApi/historization/Humidity.h>
#include <shared/plugin/yPluginApi/historization/Switch.h>
#include <shared/plugin/yPluginApi/historization/Dimmable.h>
#include <shared/plugin/yPluginApi/historization/Energy.h>
#include <shared/plugin/yPluginApi/historization/Counter.h>
#include <shared/plugin/yPluginApi/historization/PowerFactor.h>

template<class T, class K>
class COpenZWaveNodeKeywordGeneric : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordGeneric(boost::shared_ptr<K> keyword, OpenZWave::ValueID & valueId)
      : COpenZWaveNodeKeywordBase(valueId), m_keyword(keyword)
   {

   }

   virtual ~COpenZWaveNodeKeywordGeneric()
   {

   }

   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData)
   {
      m_keyword->setCommand(commandData);
      return realSendCommand<T>(m_keyword.get());
   }

   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue()
   {
      m_keyword->set(extractLastValue<T>());
      return m_keyword;
   }
   // [END] IOpenZWaveKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Switch value handler
   //--------------------------------------------------------------
   boost::shared_ptr<K> m_keyword;
};


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(OpenZWave::ValueID & vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   boost::shared_ptr<IOpenZWaveNodeKeyword> internalHistorizer = generateHistoriser(vID, homeId, nodeId, includeSystemKeywords);
   if (!internalHistorizer)
   {
      ECommandClass commandClass((int)vID.GetCommandClassId());
      std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);
      throw shared::exception::CNotSupported((boost::format("Commandclass=%1% Label=%2%") % commandClass % vLabel).str());
   }
   return internalHistorizer;
}


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::generateHistoriser(OpenZWave::ValueID & vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   try
   {
      if (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_User || (vID.GetGenre() == OpenZWave::ValueID::ValueGenre_System && includeSystemKeywords))
      {
         ECommandClass commandClass((int)vID.GetCommandClassId());
         std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);

         bool readOnly = OpenZWave::Manager::Get()->IsValueReadOnly(vID);
         bool writeOnly = OpenZWave::Manager::Get()->IsValueWriteOnly(vID);
         bool polled = OpenZWave::Manager::Get()->IsValuePolled(vID);
         bool valueSet = OpenZWave::Manager::Get()->IsValueSet(vID);
         std::string units = OpenZWave::Manager::Get()->GetValueUnits(vID);
         bool awake = OpenZWave::Manager::Get()->IsNodeAwake(homeId, nodeId);
         bool failed = OpenZWave::Manager::Get()->IsNodeFailed(homeId, nodeId);
         bool zwavePlus = OpenZWave::Manager::Get()->IsNodeZWavePlus(homeId, nodeId);

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
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CEnergy> historizer(new shared::plugin::yPluginApi::historization::CEnergy(vLabel, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CEnergy>(historizer, vID));
      }
      if (vLabel == "Total energy production" || vLabel == "Energy production today")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CEnergy> historizer(new shared::plugin::yPluginApi::historization::CEnergy(vLabel, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kCumulative));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CEnergy>(historizer, vID));
      }
      break;
   case ECommandClass::kMeterValue:
      if (vLabel == "Energy")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CEnergy> historizer(new shared::plugin::yPluginApi::historization::CEnergy(vLabel, accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CEnergy>(historizer, vID));
      }
      if (vLabel == "Power")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPower> historizer(new shared::plugin::yPluginApi::historization::CPower(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CPower>(historizer, vID));
      }
      if (vLabel == "Count")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCounter> historizer(new shared::plugin::yPluginApi::historization::CCounter(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<Poco::Int32, shared::plugin::yPluginApi::historization::CCounter>(historizer, vID));
      }
      if (vLabel == "Voltage")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CVoltage> historizer(new shared::plugin::yPluginApi::historization::CVoltage(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CVoltage>(historizer, vID));
      }
      if (vLabel == "Current")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCurrent> historizer(new shared::plugin::yPluginApi::historization::CCurrent(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CCurrent>(historizer, vID));
      }
      if (vLabel == "Power Factor")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPowerFactor> historizer(new shared::plugin::yPluginApi::historization::CPowerFactor(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CPowerFactor>(historizer, vID));
      }
      break;

   case ECommandClass::kMeterPulseValue:
      if (vLabel == "Count")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCounter> historizer(new shared::plugin::yPluginApi::historization::CCounter(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<Poco::Int32, shared::plugin::yPluginApi::historization::CCounter>(historizer, vID));
      }
      break;

   case ECommandClass::kSensorMultilevelValue:

      if (vLabel == "Temperature" || vLabel == "Water Temperature" || vLabel == "Soil Temperature")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CTemperature> historizer(new shared::plugin::yPluginApi::historization::CTemperature(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CTemperature>(historizer, vID));
      }

      if (vLabel == "Luminance")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CIllumination> historizer(new shared::plugin::yPluginApi::historization::CIllumination(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CIllumination>(historizer, vID));
      }

      if (vLabel == "Power")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPower> historizer(new shared::plugin::yPluginApi::historization::CPower(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CPower>(historizer, vID));
      }

      if (vLabel == "Relative Humidity")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CHumidity> historizer(new shared::plugin::yPluginApi::historization::CHumidity(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CHumidity>(historizer, vID));
      }

      if (vLabel == "Velocity")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSpeed> historizer(new shared::plugin::yPluginApi::historization::CSpeed(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CSpeed>(historizer, vID));
      }

      if (vLabel == "Direction")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CDirection> historizer(new shared::plugin::yPluginApi::historization::CDirection(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<Poco::Int32, shared::plugin::yPluginApi::historization::CDirection>(historizer, vID));
      }

      if (vLabel == "Barometric Pressure")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPressure> historizer(new shared::plugin::yPluginApi::historization::CPressure(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CPressure>(historizer, vID));
      }

      if (vLabel == "Rain Rate")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CRainRate> historizer(new shared::plugin::yPluginApi::historization::CRainRate(vLabel));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CRainRate>(historizer, vID));
      }

      if (vLabel == "Weight")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CWeight> historizer(new shared::plugin::yPluginApi::historization::CWeight(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CWeight>(historizer, vID));
      }

      if (vLabel == "Voltage")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CVoltage> historizer(new shared::plugin::yPluginApi::historization::CVoltage(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CVoltage>(historizer, vID));
      }
      
      if (vLabel == "Current")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCurrent> historizer(new shared::plugin::yPluginApi::historization::CCurrent(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CCurrent>(historizer, vID));
      }

      if (vLabel == "Distance")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CDistance> historizer(new shared::plugin::yPluginApi::historization::CDistance(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CDistance>(historizer, vID));
      }

      if (vLabel == "Ultraviolet")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CUv> historizer(new shared::plugin::yPluginApi::historization::CUv(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CUv>(historizer, vID));
      }

      if (vLabel == "Moisture")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CHumidity> historizer(new shared::plugin::yPluginApi::historization::CHumidity(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CHumidity>(historizer, vID));
      }

      break;

   case ECommandClass::kSwitchBinaryValue:
      if (vLabel == "Switch")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSwitch> historizer(new shared::plugin::yPluginApi::historization::CSwitch(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<bool, shared::plugin::yPluginApi::historization::CSwitch>(historizer, vID));
      }
      break;
   case ECommandClass::kSwitchMultilevelValue:
      if (vLabel == "Level")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CDimmable> historizer(new shared::plugin::yPluginApi::historization::CDimmable(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<int, shared::plugin::yPluginApi::historization::CDimmable>(historizer, vID));
      }
      break;
   case ECommandClass::kSwitchToggleBinaryValue:
      if (vLabel == "Toggle Switch")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSwitch> historizer(new shared::plugin::yPluginApi::historization::CSwitch(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<bool, shared::plugin::yPluginApi::historization::CSwitch>(historizer, vID));
      }
      break;
   case ECommandClass::kSwitchToggleMultilevelValue:
      if (vLabel == "Level")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CDimmable> historizer(new shared::plugin::yPluginApi::historization::CDimmable(vLabel, accessMode));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<int, shared::plugin::yPluginApi::historization::CDimmable>(historizer, vID));
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
         boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool> > historizerBool(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kBool), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<bool, shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool> >(historizerBool, vID));
      }

      case  OpenZWave::ValueID::ValueType_Byte:			// 8-bit unsigned value
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::UInt8> > historizerByte(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::UInt8>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<Poco::UInt8, shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::UInt8> >(historizerByte, vID));
      }

      case  OpenZWave::ValueID::ValueType_Decimal:		// Represents a non-integer value as a string, to avoid floating point accuracy issues
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> > historizerDecimal(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<double>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<double, shared::plugin::yPluginApi::historization::CSingleHistorizableData<double> >(historizerDecimal, vID));
      }

      case  OpenZWave::ValueID::ValueType_Int:			// 32-bit signed value
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int32> > historizerInt(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int32>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<Poco::Int32, shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int32> >(historizerInt, vID));
      }

      case  OpenZWave::ValueID::ValueType_List:			// List from which one item can be selected
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string> > historizerList(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kEnum), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<std::string, shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string> >(historizerList, vID));
      }

      case  OpenZWave::ValueID::ValueType_Schedule:			// Complex type used with the Climate Control Schedule command class
      {
         YADOMS_LOG(information) << "Value Schedule : ";
         break;
      }

      case  OpenZWave::ValueID::ValueType_Short:			// 16-bit signed value
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int16> > historizerShort(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int16>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kNumeric), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<Poco::Int16, shared::plugin::yPluginApi::historization::CSingleHistorizableData<Poco::Int16> >(historizerShort, vID));
      }

      case  OpenZWave::ValueID::ValueType_String:			// Text string
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string> > historizerText(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kString), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<std::string, shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string> >(historizerText, vID));
      }

      case  OpenZWave::ValueID::ValueType_Button:			// A write-only value that is the equivalent of pressing a button to send a command to a device
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool> > historizerButton(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kBool), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<bool, shared::plugin::yPluginApi::historization::CSingleHistorizableData<bool> >(historizerButton, vID));
      }

      case  OpenZWave::ValueID::ValueType_Raw:			// A collection of bytes
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string> > historizerRaw(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string>(vLabel, GetCapacity(vLabel, units, shared::plugin::yPluginApi::EKeywordDataType::kString), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute));
         return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<std::string, shared::plugin::yPluginApi::historization::CSingleHistorizableData<std::string> >(historizerRaw, vID));
      }
   }
   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}

const shared::plugin::yPluginApi::CStandardCapacity& COpenZWaveNodeKeywordFactory::GetCapacity(const std::string & name, const std::string & unit, shared::plugin::yPluginApi::EKeywordDataType dataType)
{
   m_declaredCapacities.push_back(shared::plugin::yPluginApi::CStandardCapacity(name, unit, dataType));
   return m_declaredCapacities.back();
}