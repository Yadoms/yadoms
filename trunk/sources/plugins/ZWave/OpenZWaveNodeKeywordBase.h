#pragma once

#include "IOpenZWaveNodeKeyword.h"
#include "Value.h"
#include "Manager.h"
#include <shared/exception/OutOfRange.hpp>

//--------------------------------------------------------------
/// \brief	    Base class for keywords
//--------------------------------------------------------------
class COpenZWaveNodeKeywordBase : public IOpenZWaveNodeKeyword
{
protected:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   COpenZWaveNodeKeywordBase(OpenZWave::ValueID & valueId);

public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveNodeKeywordBase();
   
   // IOpenZWaveKeyword implementation
   virtual bool sendCommand(const std::string & commandData) = 0;
   virtual std::string getLastKeywordValue() = 0;
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
      return OpenZWave::Manager::Get()->SetValue(m_valueId, data);      
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


