#pragma once

#include "OpenZWaveCommandClass.h"
#include <value_classes/Value.h>
#include "IOpenZWaveNodeKeyword.h"
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <Notification.h>
#include <Poco/Types.h>



//--------------------------------------------------------------
/// \brief  Handle a zwave enumeration
//--------------------------------------------------------------
class COpenZWaveEnumHandler : public shared::enumeration::IExtendedEnum
{
public:
   //--------------------------------------------------------------
   /// \brief	    Default constructor
   //--------------------------------------------------------------
   COpenZWaveEnumHandler(); 
   
   //--------------------------------------------------------------
   /// \brief	    Default constructor
   //--------------------------------------------------------------
   COpenZWaveEnumHandler(const std::string &str);

   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param [in]   vID   The OpenZWave valueID
   //--------------------------------------------------------------
   COpenZWaveEnumHandler(OpenZWave::ValueID & vID);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveEnumHandler();

   //shared::enumeration::IExtendedEnum implementation
   virtual const std::string & getName() const;
   virtual const std::string & toString() const;
   virtual void fromString(const std::string & val);
   virtual const std::multimap<int, std::string> getAllValuesAndStrings() const;
   virtual const std::vector<int> getAllValues() const;
   virtual const std::vector<std::string> getAllStrings() const;
   //[END] - shared::enumeration::IExtendedEnum implementation

private:
   //--------------------------------------------------------------
   /// \brief	The enumeration type name
   //--------------------------------------------------------------
   std::string m_name;

   //--------------------------------------------------------------
   /// \brief	The current string value
   //--------------------------------------------------------------
   std::string m_valueString;

   //--------------------------------------------------------------
   /// \brief	The current integer value
   //--------------------------------------------------------------
   Poco::Int32 m_valueInteger;

   //--------------------------------------------------------------
   /// \brief	The valueId
   //--------------------------------------------------------------
   OpenZWave::ValueID m_value;

   //--------------------------------------------------------------
   /// \brief	The list of all enumeration strings
   //--------------------------------------------------------------
   std::vector<std::string> m_strings;

   //--------------------------------------------------------------
   /// \brief	The list of all enumeration integers
   //--------------------------------------------------------------
   std::vector<Poco::Int32> m_integers;
};


