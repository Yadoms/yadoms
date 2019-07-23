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
	kOled0_66,
};

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class COledEspEasyConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~COledEspEasyConfiguration();

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const shared::CDataContainer& data);

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace() const;

   //--------------------------------------------------------------
   // Obtain IP address of ESPEasy firmware
   //--------------------------------------------------------------
   std::string getIPAddress() const;

   //--------------------------------------------------------------
   /// \brief	    Enum parameter
   //--------------------------------------------------------------
   EEnumType getOledType() const;

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
};

