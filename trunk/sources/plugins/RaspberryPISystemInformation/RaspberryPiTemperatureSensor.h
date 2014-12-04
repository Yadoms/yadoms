#pragma once

#include "ILoad.h"
#include "LoadKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Temperature sensor
/// \note   Use to simulate a temperature sensor, with 2 variables values :
///         - temperature : start at 25�, and vary from +- 0.0 to 1.0� at each read
///         - battery level : start at 100%, decrease by 1% at each read, to 20%
//--------------------------------------------------------------
class CRaspberryPiTemperatureSensor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CRaspberryPiTemperatureSensor(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRaspberryPiTemperatureSensor();

   // ILoad Implementation
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context);
   virtual void read();
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
   // [END] ILoad Implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_device;

   //--------------------------------------------------------------
   /// \brief	    Keyword string
   //--------------------------------------------------------------
   CLoadKeyword m_keyword;
};

