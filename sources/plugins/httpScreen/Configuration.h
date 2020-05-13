#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	An example of an enum type of parameter
//--------------------------------------------------------------
enum EScreenType
{
   kTft = 0,
   kEInk = 1
};

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CConfiguration();

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data);

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace() const;

   //--------------------------------------------------------------
   // Obtain IP address of ESPEasy firmware
   //--------------------------------------------------------------
   std::string getIPAddress() const;

   //--------------------------------------------------------------
   /// \brief	    Obtain the screen type
   //--------------------------------------------------------------
   EScreenType getSceenType() const;

   //--------------------------------------------------------------
   /// \brief	    Obtain the screen width in pixels
   //--------------------------------------------------------------
   unsigned int getSceenWidth() const;
   //--------------------------------------------------------------
   /// \brief	    Obtain the screen height in pixels
   //--------------------------------------------------------------
   unsigned int  getSceenHeight() const;

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
};

