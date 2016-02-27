#pragma once

#include "IOpenZWaveNodeKeyword.h"
#include <value_classes/Value.h>
#include <Manager.h>
#include <shared/exception/OutOfRange.hpp>
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
   explicit COpenZWaveNodeKeywordBase(OpenZWave::ValueID & valueId);

public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordBase();
   
   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData) = 0;
   virtual const shared::plugin::yPluginApi::historization::IHistorizable & getLastKeywordValue() = 0;
   // [END] IOpenZWaveKeyword implementation
      
protected:
   //--------------------------------------------------------------
   /// \brief	      Send a command.
   /// \param [in]   The data to send
   /// \return       true if command is sent (just sent, not applied), false other cases
   /// \note	      T the type of data
   //--------------------------------------------------------------
   template<class T>
   bool realSendCommand(const T & data)
   {
      try
      {
         return OpenZWave::Manager::Get()->SetValue(m_valueId, data);
      }
      catch (OpenZWave::OZWException & ex)
      {
         YADOMS_LOG(fatal) << "Fail to send command : OpenZWave exception : " << ex.what();
      }
      catch (std::exception & ex)
      {
         YADOMS_LOG(fatal) << "Fail to send command : std::exception : " << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(fatal) << "Fail to send command : unknown exception";
      }
      return false;
   }


   template<class T>
   inline T extractLastValue();

   
private:
   //--------------------------------------------------------------
   /// \brief	      The value id
   //--------------------------------------------------------------
   OpenZWave::ValueID m_valueId;   
};

template<class T>
inline T COpenZWaveNodeKeywordBase::extractLastValue()
{
   throw shared::exception::COutOfRange("This type is not supported");
}

template<>
inline bool COpenZWaveNodeKeywordBase::extractLastValue()
{
   bool value;
   OpenZWave::Manager::Get()->GetValueAsBool(m_valueId, &value);
   return value;
}


