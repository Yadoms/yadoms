#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


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
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CEmptyPluginConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CEmptyPluginConfiguration();

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const boost::shared_ptr<shared::CDataContainer> & data);

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace() const;

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // You can add your own accessors for better code readability
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   //--------------------------------------------------------------
   /// \brief	    Enum parameter
   //--------------------------------------------------------------
   EEnumType getEnumParameter() const;

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
};

