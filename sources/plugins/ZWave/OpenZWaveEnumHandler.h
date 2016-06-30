#pragma once

#include "OpenZWaveCommandClass.h"
#include <value_classes/Value.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
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
   explicit COpenZWaveEnumHandler(const std::string& str);

   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param [in]   vID   The OpenZWave valueID
   //--------------------------------------------------------------
   explicit COpenZWaveEnumHandler(OpenZWave::ValueID& vID);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COpenZWaveEnumHandler();

   //shared::enumeration::IExtendedEnum implementation
   const std::string& getName() const override;
   const std::string& toString() const override;
   void fromString(const std::string& val) override;
   const std::multimap<int, std::string> getAllValuesAndStrings() const override;
   const std::vector<int> getAllValues() const override;
   const std::vector<std::string> getAllStrings() const override;
   //[END] - shared::enumeration::IExtendedEnum implementation

   //--------------------------------------------------------------
   /// \brief	    Get the text associated to a value
   /// \param [in]   integerValueInString   The enum value as string : "0", "4",...
   /// \param [out]  result                  The associated text
   /// \return true if value has been found, false otherwize
   //--------------------------------------------------------------
   bool getTextOfValue(const std::string& integerValueInString,
                       std::string& result);

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

