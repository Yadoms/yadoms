#include "stdafx.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include <shared/exception/NotSupported.hpp>
#include "OpenZWaveNodeKeywordSwitch.h"
#include "OpenZWaveNodeKeywordSensorMultilevelValue.h"
#include "OpenZWaveCommandClass.h"


#include <value_classes/ValueBool.h>
#include <value_classes/ValueDecimal.h>


boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeKeywordFactory::createKeyword(ECommandClass commandClass, const std::string & keyword, OpenZWave::ValueID & value)
{
   switch(commandClass())
   {
   case ECommandClass::kSwitchBinaryValue:
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordSwitch(keyword + "." + commandClass.toString(), value));

   case ECommandClass::kSensorMultilevelValue:
      return boost::shared_ptr<IOpenZWaveNodeKeyword>(new COpenZWaveNodeKeywordSensorMultilevelValue(keyword + "." + commandClass.toString(), value));

    default:
      throw shared::exception::CNotSupported(commandClass.toString());
      break;
   }
}


std::string GenerateDeviceStringID(uint32 homeId, uint8 nodeId)
{
   std::stringstream sstr;
   sstr << homeId << "." << (int)nodeId;
   return sstr.str();
}


boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNodeKeywordFactory::generateHistoriser(OpenZWave::Notification const* _notification)
{
   try
   {
      //get all glocbal informations (for all notifications)
      OpenZWave::ValueID vID = _notification->GetValueID();
      ECommandClass commandClass((int)vID.GetCommandClassId());

      OpenZWave::Node rawNode(_notification->GetHomeId(), _notification->GetNodeId());
      OpenZWave::Value * rawValue = rawNode.GetValue(vID);



      std::string id = GenerateDeviceStringID(_notification->GetHomeId(), _notification->GetNodeId());
      std::string vLabel = OpenZWave::Manager::Get()->GetValueLabel(vID);


      bool readOnly = OpenZWave::Manager::Get()->IsValueReadOnly(vID);
      bool writeOnly = OpenZWave::Manager::Get()->IsValueWriteOnly(vID);
      bool polled = OpenZWave::Manager::Get()->IsValuePolled(vID);
      bool valueSet = OpenZWave::Manager::Get()->IsValueSet(vID);
      std::string units = OpenZWave::Manager::Get()->GetValueUnits(vID);
      
      bool awake = OpenZWave::Manager::Get()->IsNodeAwake(_notification->GetHomeId(), _notification->GetNodeId());
      bool failed = OpenZWave::Manager::Get()->IsNodeFailed(_notification->GetHomeId(), _notification->GetNodeId());
      bool zwavePlus = OpenZWave::Manager::Get()->IsNodeZWavePlus(_notification->GetHomeId(), _notification->GetNodeId());

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

      YADOMS_LOG(information) << "[" << id << "] " << vLabel << ":" << commandClass.toString() 
         << " [Genre=" << genre << "]"
         << " [ReadOnly=" << readOnly << "]"
         << " [WriteOnly= " << writeOnly << "]"
         << " [Polled= " << polled << "]"
         << " [Set= " << valueSet << "]"
         << " [Units= " << units << "]"
         << " [Awake= " << awake << "]"
         << " [Failed= " << failed << "]"
         << " [ZWave+s= " << zwavePlus << "]"
         ;

      

      bool bValue;
      Poco::UInt8 byteValue;
      Poco::Int16 shortValue;
      Poco::Int32 intValue;
      float floatValue;
      std::string stringValue;
      std::vector<std::string> vectorString;
      std::vector<int> vectorInt;


      

      switch (vID.GetType())
      {
      case  OpenZWave::ValueID::ValueType_Bool:			// Boolean, true or false
         if (OpenZWave::Manager::Get()->GetValueAsBool(vID, &bValue))
            YADOMS_LOG(information) << "Value BOOL = " << (bValue?"true":"false");
         else
            YADOMS_LOG(information) << "Value BOOL : N/A";
         break;

      case  OpenZWave::ValueID::ValueType_Byte:			// 8-bit unsigned value
         if (OpenZWave::Manager::Get()->GetValueAsByte(vID, &byteValue))
            YADOMS_LOG(information) << "Value Byte = " << (int)byteValue;
         else
            YADOMS_LOG(information) << "Value Byte : N/A";
         break;

      case  OpenZWave::ValueID::ValueType_Decimal:		// Represents a non-integer value as a string, to avoid floating point accuracy issues
         if (OpenZWave::Manager::Get()->GetValueAsFloat(vID, &floatValue))
            YADOMS_LOG(information) << "Value Decimal = " << floatValue;
         else
            YADOMS_LOG(information) << "Value Decimal : N/A";
         break;

      case  OpenZWave::ValueID::ValueType_Int:			// 32-bit signed value
         if (OpenZWave::Manager::Get()->GetValueAsInt(vID, &intValue))
            YADOMS_LOG(information) << "Value Int = " << intValue;
         else
            YADOMS_LOG(information) << "Value Int : N/A ";
         break;

      case  OpenZWave::ValueID::ValueType_List:			// List from which one item can be selected
         OpenZWave::Manager::Get()->GetValueListItems(vID, &vectorString);
         OpenZWave::Manager::Get()->GetValueListValues(vID, &vectorInt);

         if (OpenZWave::Manager::Get()->GetValueListSelection(vID, &intValue))
         {
            std::string selectedText = "<unknown>";
            for (int i = 0; i < vectorInt.size(); ++i)
            {
               if (vectorInt[i] == intValue)
               {
                  selectedText= vectorString[i];
               }
            }
            YADOMS_LOG(information) << "Value List = " << selectedText;
         }
         else
            YADOMS_LOG(information) << "Value List : N/A ";

         break;
      
      case  OpenZWave::ValueID::ValueType_Schedule:			// Complex type used with the Climate Control Schedule command class
         YADOMS_LOG(information) << "Value Schedule : ";
         break;
      
      case  OpenZWave::ValueID::ValueType_Short:			// 16-bit signed value
         if (OpenZWave::Manager::Get()->GetValueAsShort(vID, &shortValue))
            YADOMS_LOG(information) << "Value Short = " << shortValue;
         else
            YADOMS_LOG(information) << "Value Short : N/A ";
         break;

      case  OpenZWave::ValueID::ValueType_String:			// Text string
         if (OpenZWave::Manager::Get()->GetValueAsString(vID, &stringValue))
            YADOMS_LOG(information) << "Value String = " << stringValue;
         else
            YADOMS_LOG(information) << "Value String : N/A ";
         break;

      case  OpenZWave::ValueID::ValueType_Button:			// A write-only value that is the equivalent of pressing a button to send a command to a device
         YADOMS_LOG(information) << "Value Button : N/A";
         break;

      case  OpenZWave::ValueID::ValueType_Raw:			// A collection of bytes
         YADOMS_LOG(information) << "Value Raw : N/A";
         break;
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
   /*
   OpenZWave::ValueBool *boolValue = dynamic_cast<OpenZWave::ValueBool*>(rawValue);
   if (boolValue != NULL)
   {

      YADOMS_LOG(information) << "Value BOOL : ";
      YADOMS_LOG(information) << "    Id =" << id;
      YADOMS_LOG(information) << "    Label =" << vLabel;
      YADOMS_LOG(information) << "    ReadOnly =" << boolValue->IsReadOnly();
      YADOMS_LOG(information) << "    WriteOnly =" << boolValue->IsWriteOnly();
      YADOMS_LOG(information) << "    Value =" << boolValue->GetValue() << " " << boolValue->GetUnits();
   }

   OpenZWave::ValueDecimal *decimalValue = dynamic_cast<OpenZWave::ValueDecimal*>(rawValue);
   if (decimalValue != NULL)
   {

      YADOMS_LOG(information) << "Value DECIMAL : ";
      YADOMS_LOG(information) << "    Id =" << id;
      YADOMS_LOG(information) << "    Label =" << vLabel;
      YADOMS_LOG(information) << "    ReadOnly =" << decimalValue->IsReadOnly();
      YADOMS_LOG(information) << "    WriteOnly =" << decimalValue->IsWriteOnly();
      YADOMS_LOG(information) << "    Value =" << decimalValue->GetValue() << " " << decimalValue->GetUnits();
   }
   */

   return boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable>();
}