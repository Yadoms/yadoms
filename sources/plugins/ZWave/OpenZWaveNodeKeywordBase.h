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
   virtual boost::shared_ptr<shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo> & getTypeInformation() override;
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
   boost::shared_ptr<shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo> m_typeInformation;
};


//--------------------------------------------------------------
template <class T>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const T& data)
{
   try
   {
      return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what() ;
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception" ;
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
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what() ;
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception" ;
   }
   return false;
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::UInt64& data)
{
   try
   {
      return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int32>(data));
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what() ;
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception" ;
   }
   return false;
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const Poco::Int64& data)
{
   try
   {
      return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<Poco::Int32>(data));
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what() ;
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception" ;
   }
   return false;
}

template <>
inline bool COpenZWaveNodeKeywordBase::realSendCommand(const double& data)
{
   try
   {
      return OpenZWave::Manager::Get()->SetValue(m_valueId, static_cast<float>(data));
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : OpenZWave exception : " << ex.what() ;
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : std::exception : " << ex.what() ;
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown exception" ;
   }
   return false;
}


template <class T>
inline T COpenZWaveNodeKeywordBase::extractLastValue()
{
   YADOMS_LOG(information) << "COpenZWaveNodeKeywordBase::extractLastValue : generic type is not supported. Only overriden types are allowed" ;
   std::string value;
   OpenZWave::Manager::Get()->GetValueAsString(m_valueId, &value);
   return boost::lexical_cast<T>(value);
}

template <>
inline bool COpenZWaveNodeKeywordBase::extractLastValue()
{
   bool value;
   OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &value);
   return value;
}

template <>
inline Poco::UInt8 COpenZWaveNodeKeywordBase::extractLastValue()
{
   Poco::UInt8 value;
   OpenZWave::Manager::Get()->GetValueAsByte(m_valueId, &value);
   return value;
}

template <>
inline float COpenZWaveNodeKeywordBase::extractLastValue()
{
   float value;
   OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &value);
   return value;
}


template <>
inline double COpenZWaveNodeKeywordBase::extractLastValue()
{
   float value;
   OpenZWave::Manager::Get()->GetValueAsFloat(m_valueId, &value);
   return value;
}


template <>
inline Poco::Int32 COpenZWaveNodeKeywordBase::extractLastValue()
{
   Poco::Int32 value;
   OpenZWave::Manager::Get()->GetValueAsInt(m_valueId, &value);
   return value;
}


template <>
inline Poco::Int16 COpenZWaveNodeKeywordBase::extractLastValue()
{
   Poco::Int16 value;
   OpenZWave::Manager::Get()->GetValueAsShort(m_valueId, &value);
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

