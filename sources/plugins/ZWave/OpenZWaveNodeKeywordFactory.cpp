#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveCommandClass.h"


#include <value_classes/ValueBool.h>
#include <value_classes/ValueDecimal.h>
#include <Manager.h>

#include <shared/Log.h>
#include "OpenZWaveNodeKeywordBase.h"

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

#include "OpenZWaveHelpers.h"

std::vector<shared::plugin::yPluginApi::CStandardCapacity> COpenZWaveNodeKeywordFactory::m_declaredCapacities;

template<class T>
class COpenZWaveNodeKeywordGeneric : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordGeneric(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > keyword, OpenZWave::ValueID & valueId)
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
      return realSendCommand<T>(m_keyword->get());
   }

   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue()
   {
      m_keyword->set(extractLastValue<T>());
      return m_keyword;
   }
   // [END] IOpenZWaveKeyword implementation

   static boost::shared_ptr<IOpenZWaveNodeKeyword> create(boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > historizer, OpenZWave::ValueID & vID)
   {
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<T>(historizer, vID));
   }

   static boost::shared_ptr<IOpenZWaveNodeKeyword> createFromDataType(OpenZWave::ValueID & vID, const std::string & vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode, const std::string &units, shared::plugin::yPluginApi::EKeywordDataType dataType, shared::plugin::yPluginApi::historization::EMeasureType measureType = shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute)
   {
      boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > historizer(new shared::plugin::yPluginApi::historization::CSingleHistorizableData<T>(COpenZWaveHelpers::GenerateKeywordName(vID), COpenZWaveNodeKeywordFactory::getCapacity(vLabel, units, dataType), accessMode, measureType));
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordGeneric<T>(historizer, vID));
   }


private:
   //--------------------------------------------------------------
   /// \brief	    Switch value handler
   //--------------------------------------------------------------
   boost::shared_ptr< shared::plugin::yPluginApi::historization::CSingleHistorizableData<T> > m_keyword;
};


class COpenZWaveNodeKeywordDimmable : public COpenZWaveNodeKeywordBase
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordDimmable(OpenZWave::ValueID & valueId, const std::string & vLabel, shared::plugin::yPluginApi::EKeywordAccessMode accessMode)
      : COpenZWaveNodeKeywordBase(valueId), m_keyword(new shared::plugin::yPluginApi::historization::CDimmable(COpenZWaveHelpers::GenerateKeywordName(valueId), accessMode))
   {

   }

   virtual ~COpenZWaveNodeKeywordDimmable()
   {

   }

   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData)
   {
      m_keyword->set(commandData);
      return realSendCommand<int>(m_keyword->switchLevel());
   }

   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue()
   {
      m_keyword->set(extractLastValue<int>());
      return m_keyword;
   }
   // [END] IOpenZWaveKeyword implementation

private:
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CDimmable> m_keyword;
};

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(OpenZWave::ValueID & vID, Poco::UInt32 homeId, Poco::UInt8 nodeId, bool includeSystemKeywords)
{
   return generateHistoriser(vID, homeId, nodeId, includeSystemKeywords);
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
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CEnergy> historizer(new shared::plugin::yPluginApi::historization::CEnergy(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<long>::create(historizer, vID);
      }
      if (vLabel == "Total energy production" || vLabel == "Energy production today")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CEnergy> historizer(new shared::plugin::yPluginApi::historization::CEnergy(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode, shared::plugin::yPluginApi::historization::EMeasureType::kCumulative));
         return COpenZWaveNodeKeywordGeneric<long>::create(historizer, vID);
      }
      break;
   case ECommandClass::kMeterValue:
      if (vLabel == "Energy")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CEnergy> historizer(new shared::plugin::yPluginApi::historization::CEnergy(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<long>::create(historizer, vID);
      }
      if (vLabel == "Power")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPower> historizer(new shared::plugin::yPluginApi::historization::CPower(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Count")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCounter> historizer(new shared::plugin::yPluginApi::historization::CCounter(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<Poco::Int64>::create(historizer, vID);
      }
      if (vLabel == "Voltage")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CVoltage> historizer(new shared::plugin::yPluginApi::historization::CVoltage(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Current")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCurrent> historizer(new shared::plugin::yPluginApi::historization::CCurrent(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      if (vLabel == "Power Factor")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPowerFactor> historizer(new shared::plugin::yPluginApi::historization::CPowerFactor(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      break;

   case ECommandClass::kMeterPulseValue:
      if (vLabel == "Count")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCounter> historizer(new shared::plugin::yPluginApi::historization::CCounter(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<Poco::Int64>::create(historizer, vID);
      }
      break;

   case ECommandClass::kSensorMultilevelValue:

      if (vLabel == "Temperature" || vLabel == "Water Temperature" || vLabel == "Soil Temperature")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CTemperature> historizer(new shared::plugin::yPluginApi::historization::CTemperature(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Luminance")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CIllumination> historizer(new shared::plugin::yPluginApi::historization::CIllumination(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Power")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPower> historizer(new shared::plugin::yPluginApi::historization::CPower(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Relative Humidity")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CHumidity> historizer(new shared::plugin::yPluginApi::historization::CHumidity(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Velocity")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSpeed> historizer(new shared::plugin::yPluginApi::historization::CSpeed(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Direction")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CDirection> historizer(new shared::plugin::yPluginApi::historization::CDirection(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<Poco::Int32>::create(historizer, vID);
      }

      if (vLabel == "Barometric Pressure")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CPressure> historizer(new shared::plugin::yPluginApi::historization::CPressure(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Rain Rate")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CRainRate> historizer(new shared::plugin::yPluginApi::historization::CRainRate(COpenZWaveHelpers::GenerateKeywordName(vID)));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Weight")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CWeight> historizer(new shared::plugin::yPluginApi::historization::CWeight(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Voltage")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CVoltage> historizer(new shared::plugin::yPluginApi::historization::CVoltage(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }
      
      if (vLabel == "Current")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CCurrent> historizer(new shared::plugin::yPluginApi::historization::CCurrent(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Distance")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CDistance> historizer(new shared::plugin::yPluginApi::historization::CDistance(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Ultraviolet")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CUv> historizer(new shared::plugin::yPluginApi::historization::CUv(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      if (vLabel == "Moisture")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CHumidity> historizer(new shared::plugin::yPluginApi::historization::CHumidity(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
         return COpenZWaveNodeKeywordGeneric<double>::create(historizer, vID);
      }

      break;

   case ECommandClass::kSwitchBinaryValue:
      if (vLabel == "Switch")
      {
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSwitch> historizer(new shared::plugin::yPluginApi::historization::CSwitch(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
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
         boost::shared_ptr<shared::plugin::yPluginApi::historization::CSwitch> historizer(new shared::plugin::yPluginApi::historization::CSwitch(COpenZWaveHelpers::GenerateKeywordName(vID), accessMode));
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
         return COpenZWaveNodeKeywordGeneric<std::string>::createFromDataType(vID, vLabel, accessMode, units, shared::plugin::yPluginApi::EKeywordDataType::kEnum);
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
   m_declaredCapacities.push_back(shared::plugin::yPluginApi::CStandardCapacity(name, unit, dataType));
   return m_declaredCapacities.back();
}