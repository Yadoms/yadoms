#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


enum EPairingMode
{
   kAuto = 0,
   kManual
};

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class Configuration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~Configuration();

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data);

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace() const;

   /**
   * \brief  Get Device IP address
   * \return Returns Device IP address
   */
   std::string getIPAddress() const;
   /**
   * \brief  Set Device IP address
   * \param[in] ipAddress Device IP Address
   */
   void setIPAddress(const std::string& ipAddress);
   /**
   * \brief  get pairing mode
   * \return Returns pairing mode
   */
   EPairingMode getPairingMode() const;
private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
   std::string m_ipAddress;
};
