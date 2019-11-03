#pragma once
#include "IOpenZWaveNodeKeyword.h"
#include <Manager.h>
#include "OpenZWaveEnumHandler.h"
#include <shared/Log.h>
//--------------------------------------------------------------
/// \brief	    Base class for keywords
//--------------------------------------------------------------
class COpenZWaveNodeKeywordBase : public IOpenZWaveNodeKeyword
{
protected:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   explicit COpenZWaveNodeKeywordBase(OpenZWave::ValueID& valueId);

public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordBase();

   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string& commandData) = 0;
   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> getLastKeywordValue() = 0;
   virtual void updateValue(OpenZWave::ValueID& value);
   virtual shared::CDataContainer serialize();
   virtual boost::shared_ptr<shared::plugin::yPluginApi::typeInfo::ITypeInfo> & getTypeInformation() override;
   int getIndex() override;
   ECommandClass getCommandClass() override;
   // [END] IOpenZWaveKeyword implementation

protected:
   //--------------------------------------------------------------
   /// \brief	      Send a command.
   /// \param [in]   The data to send
   /// \return       true if command is sent (just sent, not applied), false other cases
   /// \note	      T the type of data
   //--------------------------------------------------------------
   template <class T>
   inline bool realSendCommand(const T& data);

   //--------------------------------------------------------------
   /// \brief	      Send a button command (press).
   /// \return       true if command is sent (just sent, not applied), false other cases
   //--------------------------------------------------------------
   bool pressButton();

   //--------------------------------------------------------------
   /// \brief	      Send a button command (press).
   /// \return       true if command is sent (just sent, not applied), false other cases
   //--------------------------------------------------------------
   bool releaseButton();

   //--------------------------------------------------------------
   /// \brief	      Extract a typed value from the OpenZWave::ValueID container 
   /// \return       The data contained in OpenZWave::ValueID, in the goot type
   /// \note	      T the type of data
   //--------------------------------------------------------------
   template <class T>
   inline T extractLastValue();

   //--------------------------------------------------------------
   /// \brief	      Get the unit of the data
   /// \return       The data unit
   //--------------------------------------------------------------
   const std::string getUnit();

private:
   //--------------------------------------------------------------
   /// \brief	      The value id
   //--------------------------------------------------------------
   OpenZWave::ValueID m_valueId;

   //--------------------------------------------------------------
   /// \brief	      The type information
   //--------------------------------------------------------------
   boost::shared_ptr<shared::plugin::yPluginApi::typeInfo::ITypeInfo> m_typeInformation;
};


//--------------------------------------------------------------
template <class T>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const T& data)
{
   try
   {
      std::string s = (boost::format("%1%") % data).str();
      return OpenZWave::Manager::Get()->SetValue(m_valueId, s);
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const COpenZWaveEnumHandler& data)
{
   try
   {
      if (!OpenZWave::Manager::Get()->SetValueListSelection(m_valueId, data.toString()))
      {
         std::string value;
         if (COpenZWaveEnumHandler(m_valueId).getTextOfValue(data.toString(), value))
         {
            //in some cases the data do not contains the text, it contains the value
            return OpenZWave::Manager::Get()->SetValueListSelection(m_valueId, value);
         }
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}



template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const bool & data)
{
   try
   {
      switch (m_valueId.GetType())
      {
      case OpenZWave::ValueID::ValueType_Bool:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
      }
      case OpenZWave::ValueID::ValueType_Byte:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::UInt8>(data ? 1 : 0));
      }
      case OpenZWave::ValueID::ValueType_Decimal:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<float>(data ? 1.0f : 0.0f));
      }
      case OpenZWave::ValueID::ValueType_Int:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int32>(data ? 1 : 0));
      }
      case OpenZWave::ValueID::ValueType_Short:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int16>(data ? 1 : 0));
      }
      case OpenZWave::ValueID::ValueType_String:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (boost::format("%1%") % data).str());
      }
      default:
         YADOMS_LOG(error) << "Fail to send command from 'bool' to OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType());
         break;
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::UInt8& data)
{
   try
   {
      switch (m_valueId.GetType())
      {
      case OpenZWave::ValueID::ValueType_Bool:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (data == 1));
      }
      case OpenZWave::ValueID::ValueType_Byte:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
      }
      case OpenZWave::ValueID::ValueType_Decimal:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<float>(data));
      }
      case OpenZWave::ValueID::ValueType_Int:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int32>(data));
      }
      case OpenZWave::ValueID::ValueType_Short:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int16>(data));
      }
      case OpenZWave::ValueID::ValueType_String:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (boost::format("%1%") % data).str());
      }
      default:
         YADOMS_LOG(error) << "Fail to send command from 'UInt8' to OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType());
         break;
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}


template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::Int8& data)
{
   return realSendCommand<Poco::UInt8>(static_cast<Poco::UInt8>(data));
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::Int16& data)
{
   try
   {
      switch (m_valueId.GetType())
      {
      case OpenZWave::ValueID::ValueType_Bool:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (data == 1));
      }
      case OpenZWave::ValueID::ValueType_Byte:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::UInt8>(data));
      }
      case OpenZWave::ValueID::ValueType_Decimal:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<float>(data));
      }
      case OpenZWave::ValueID::ValueType_Int:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int32>(data));
      }
      case OpenZWave::ValueID::ValueType_Short:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
      }
      case OpenZWave::ValueID::ValueType_String:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (boost::format("%1%") % data).str());
      }
      default:
         YADOMS_LOG(error) << "Fail to send command from 'Int16' to OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType());
         break;
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}


template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::UInt16& data)
{
   return realSendCommand<Poco::Int16>(static_cast<Poco::Int16>(data));
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::Int32& data)
{
   try
   {
      switch (m_valueId.GetType())
      {
      case OpenZWave::ValueID::ValueType_Bool:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (data == 1));
      }
      case OpenZWave::ValueID::ValueType_Byte:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::UInt8>(data));
      }
      case OpenZWave::ValueID::ValueType_Decimal:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<float>(data));
      }
      case OpenZWave::ValueID::ValueType_Int:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
      }
      case OpenZWave::ValueID::ValueType_Short:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int16>(data));
      }
      case OpenZWave::ValueID::ValueType_String:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (boost::format("%1%") % data).str());
      }
      default:
         YADOMS_LOG(error) << "Fail to send command from 'Int32' to OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType());
         break;
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}


template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::UInt32& data)
{
   return realSendCommand<Poco::Int32>(static_cast<Poco::Int32>(data));
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::UInt64& data)
{
   return realSendCommand<Poco::Int32>(static_cast<Poco::Int32>(data));
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::Int64& data)
{
   return realSendCommand<Poco::Int32>(static_cast<Poco::Int32>(data));
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const float& data)
{
   try
   {
      switch (m_valueId.GetType())
      {
      case OpenZWave::ValueID::ValueType_Bool:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (data >= 0.99f && data <= 1.01));
      }
      case OpenZWave::ValueID::ValueType_Byte:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::UInt8>(data));
      }
      case OpenZWave::ValueID::ValueType_Decimal:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
      }
      case OpenZWave::ValueID::ValueType_Int:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int32>(data));
      }
      case OpenZWave::ValueID::ValueType_Short:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int16>(data));
      }
      case OpenZWave::ValueID::ValueType_String:
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, (boost::format("%1%") % data).str());
      }
      default:
         YADOMS_LOG(error) << "Fail to send command from 'float' to OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType());
         break;
      }
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception";
   }
   return false;
}


template <class T>
inline T COpenZWaveNodeKeywordBase::extractLastValue()
{
   YADOMS_LOG(information) << "COpenZWaveNodeKeywordBase::extractLastValue : generic type is not supported. Only overriden types are allowed";
   std::string value;
   OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &value);
   return boost::lexical_cast<T>(value);
}

template <>
inline bool COpenZWaveNodeKeywordBase::extractLastValue()
{
   bool value;

   switch (m_valueId.GetType())
   {
   case OpenZWave::ValueID::ValueType_Bool:
   {
      OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &value);
      break;
   }

   case OpenZWave::ValueID::ValueType_Byte:
   {
      //make conversion betwwen "byte" to "bool", assume 1=true and 0=false
      unsigned char bv;
      OpenZWave::Manager::Get()->GetValueAsByte(m_valueId, &bv);
      value = (bv == 1);
      break;
   }

   case OpenZWave::ValueID::ValueType_Decimal:
   {
      float fv;
      OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &fv);
      value = (fv >= 0.99f && fv <= 1.01);
      break;
   }

   case OpenZWave::ValueID::ValueType_Int:
   {
      //make conversion betwwen "int" to "bool", assume 1=true and 0=false
      int iv;
      OpenZWave::Manager::Get()->GetValueAsInt(m_valueId, &iv);
      value = (iv == 1);
      break;
   }

   case OpenZWave::ValueID::ValueType_Short:
   {
      //make conversion betwwen "short" to "bool", assume 1=true and 0=false
      short sv;
      OpenZWave::Manager::Get()->GetValueAsShort(m_valueId, &sv);
      value = (sv == 1);
      break;
   }

   case OpenZWave::ValueID::ValueType_String:
   {
      //make conversion betwwen "string" to "bool", assume true=("1" || "true")
      std::string stv;
      OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &stv);
      value = (boost::iequals(stv, "1") || boost::iequals(stv, "true"));
      break;
   }

   default:
      YADOMS_LOG(warning) << "COpenZWaveNodeKeywordBase::extractLastValue cannot convert from OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType()) << " to Bool";
      //set default value. This avoid exception and don't make code fail
      value = false;
      break;
   }
   return value;
}

template <>
inline Poco::UInt8 COpenZWaveNodeKeywordBase::extractLastValue()
{
   Poco::UInt8 value;

   switch (m_valueId.GetType())
   {
   case OpenZWave::ValueID::ValueType_Byte:
   {
      OpenZWave::Manager::Get()->GetValueAsByte(m_valueId, &value);
      break;
   }

   case OpenZWave::ValueID::ValueType_Bool:
   {
      bool bv;
      OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &bv);
      value = (bv ? 1 : 0);
      break;
   }

   case OpenZWave::ValueID::ValueType_Decimal:
   {
      float fv;
      OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &fv);
      value = static_cast<Poco::UInt8>(fv);
      break;
   }

   case OpenZWave::ValueID::ValueType_Int:
   {
      //make conversion betwwen "int" to "Uint8", assume 1=true and 0=false
      int iv;
      OpenZWave::Manager::Get()->GetValueAsInt(m_valueId, &iv);
      value = static_cast<Poco::UInt8>(iv);
      break;
   }

   case OpenZWave::ValueID::ValueType_Short:
   {
      //make conversion betwwen "short" to "Uint8", assume 1=true and 0=false
      short sv;
      OpenZWave::Manager::Get()->GetValueAsShort(m_valueId, &sv);
      value = static_cast<Poco::UInt8>(sv);
      break;
   }

   case OpenZWave::ValueID::ValueType_String:
   {
      //make conversion betwwen "string" to "Uint8", assume true=("1" || "true")
      std::string stv;
      OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &stv);
      value = boost::lexical_cast<Poco::UInt8>(stv);
      break;
   }

   default:
      YADOMS_LOG(warning) << "COpenZWaveNodeKeywordBase::extractLastValue cannot convert from OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType()) << " to Byte";
      //set default value. This avoid exception and don't make code fail
      value = 0;
      break;
   }
   return value;
}

template <>
inline float COpenZWaveNodeKeywordBase::extractLastValue()
{
   float value;

   switch (m_valueId.GetType())
   {
   case OpenZWave::ValueID::ValueType_Decimal:
   {
      OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &value);
      break;
   }

   case OpenZWave::ValueID::ValueType_Byte:
   {
      Poco::UInt8 uv;
      OpenZWave::Manager::Get()->GetValueAsByte(m_valueId, &uv);
      value = static_cast<float>(uv);
      break;
   }

   case OpenZWave::ValueID::ValueType_Bool:
   {
      bool bv;
      OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &bv);
      value = (bv ? 1.0f : 0.0f);
      break;
   }


   case OpenZWave::ValueID::ValueType_Int:
   {
      //make conversion betwwen "int" to "float", assume 1=true and 0=false
      int iv;
      OpenZWave::Manager::Get()->GetValueAsInt(m_valueId, &iv);
      value = static_cast<float>(iv);
      break;
   }

   case OpenZWave::ValueID::ValueType_Short:
   {
      //make conversion betwwen "short" to "float", assume 1=true and 0=false
      short sv;
      OpenZWave::Manager::Get()->GetValueAsShort(m_valueId, &sv);
      value = static_cast<float>(sv);
      break;
   }

   case OpenZWave::ValueID::ValueType_String:
   {
      //make conversion betwwen "string" to "float"
      std::string stv;
      OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &stv);
      value = boost::lexical_cast<float>(stv);
      break;
   }

   default:
      YADOMS_LOG(warning) << "COpenZWaveNodeKeywordBase::extractLastValue cannot convert from OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType()) << " to Float";
      //set default value. This avoid exception and don't make code fail
      value = 0.0f;
      break;
   }
   return value;
}


template <>
inline double COpenZWaveNodeKeywordBase::extractLastValue()
{
   return extractLastValue<float>();
}


template <>
inline Poco::Int32 COpenZWaveNodeKeywordBase::extractLastValue()
{
   Poco::Int32 value;

   switch (m_valueId.GetType())
   {
   case OpenZWave::ValueID::ValueType_Int:
   {
      OpenZWave::Manager::Get()->GetValueAsInt(m_valueId, &value);
      break;
   }
   case OpenZWave::ValueID::ValueType_Byte:
   {
      Poco::UInt8 v;
      OpenZWave::Manager::Get()->GetValueAsByte(m_valueId, &v);
      value = static_cast<Poco::Int32>(v);
      break;
   }

   case OpenZWave::ValueID::ValueType_Bool:
   {
      bool bv;
      OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &bv);
      value = (bv ? 1 : 0);
      break;
   }

   case OpenZWave::ValueID::ValueType_Decimal:
   {
      float fv;
      OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &fv);
      value = static_cast<Poco::Int32>(fv);
      break;
   }

   case OpenZWave::ValueID::ValueType_Short:
   {
      short sv;
      OpenZWave::Manager::Get()->GetValueAsShort(m_valueId, &sv);
      value = static_cast<Poco::Int32>(sv);
      break;
   }

   case OpenZWave::ValueID::ValueType_String:
   {
      std::string stv;
      OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &stv);
      value = boost::lexical_cast<Poco::Int32>(stv);
      break;
   }

   default:
      YADOMS_LOG(warning) << "COpenZWaveNodeKeywordBase::extractLastValue cannot convert from OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType()) << " to Int32";
      //set default value. This avoid exception and don't make code fail
      value = 0;
      break;
   }
   return value;
}


template <>
inline Poco::Int16 COpenZWaveNodeKeywordBase::extractLastValue()
{
   Poco::Int16 value;

   switch (m_valueId.GetType())
   {
   case OpenZWave::ValueID::ValueType_Byte:
   {
      Poco::UInt8 v;
      OpenZWave::Manager::Get()->GetValueAsByte(m_valueId, &v);
      value = static_cast<Poco::Int16>(v);
      break;
   }

   case OpenZWave::ValueID::ValueType_Bool:
   {
      bool bv;
      OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &bv);
      value = (bv ? 1 : 0);
      break;
   }

   case OpenZWave::ValueID::ValueType_Decimal:
   {
      float fv;
      OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &fv);
      value = static_cast<Poco::Int16>(fv);
      break;
   }

   case OpenZWave::ValueID::ValueType_Int:
   {
      Poco::Int32 v;
      OpenZWave::Manager::Get()->GetValueAsInt(m_valueId, &v);
      value = static_cast<Poco::Int16>(v);
      break;
   }

   case OpenZWave::ValueID::ValueType_Short:
   {
      OpenZWave::Manager::Get()->GetValueAsShort(m_valueId, &value);
      break;
   }

   case OpenZWave::ValueID::ValueType_String:
   {
      std::string stv;
      OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &stv);
      value = boost::lexical_cast<Poco::Int16>(stv);
      break;
   }

   default:
      YADOMS_LOG(warning) << "COpenZWaveNodeKeywordBase::extractLastValue cannot convert from OpenZWave::ValueID::ValueType =" << (int)(m_valueId.GetType()) << " to Short";
      //set default value. This avoid exception and don't make code fail
      value = 0;
      break;
   }
   return value;
}


template <>
inline std::string COpenZWaveNodeKeywordBase::extractLastValue()
{
   std::string value;
   OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &value);
   return value;
}


template <>
inline COpenZWaveEnumHandler COpenZWaveNodeKeywordBase::extractLastValue()
{
   return COpenZWaveEnumHandler(m_valueId);
}

