#pragma once

#include "IConfigurationSerializer.h"


//--------------------------------------------------------------
/// \class Hardware plugin configuration factory
//--------------------------------------------------------------
class CConfigurationFactory
{
   // This class can only be used by the configuration
   friend class CHardwarePluginConfiguration;

private:
   //--------------------------------------------------------------
   /// \brief	   Serializer creation
   /// \return    A new serializer instance
   //--------------------------------------------------------------
   static boost::shared_ptr<IConfigurationSerializer> createSerializer();
};
