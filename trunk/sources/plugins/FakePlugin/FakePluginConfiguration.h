#pragma once

#include <shared/plugin/configuration/Configuration.h>
#include <shared/plugin/ImplementationHelper.h>


//--------------------------------------------------------------
/// \brief	An example of an enum type of parameter
//--------------------------------------------------------------
enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

//--------------------------------------------------------------
/// \brief	Configuration of the fake plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CFakePluginConfiguration : public shared::plugin::configuration::CConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakePluginConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // You can add your own accessors here for better code readability
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   //--------------------------------------------------------------
   /// \brief	    Serial port getter 
   //--------------------------------------------------------------
   std::string getSerialPort() const;

   //--------------------------------------------------------------
   /// \brief	    Enum parameter
   //--------------------------------------------------------------
   EEnumType getEnumParameter() const;
};

