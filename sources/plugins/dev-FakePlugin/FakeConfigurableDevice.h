#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Fake configurable device
/// \note   User can configurate the counter divider (increment each N read operations)
//--------------------------------------------------------------
class CFakeConfigurableDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   explicit CFakeConfigurableDevice(const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeConfigurableDevice();

   //--------------------------------------------------------------
   /// \brief	            Declare device and associated keywords if necessary
   /// \param[in] api      yPluginApi context
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read();

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] api      yPluginApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor device name
   /// \return     The device name
   //--------------------------------------------------------------
   const std::string& getDeviceName() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::string& getModel();

   //--------------------------------------------------------------
   /// \brief	            Get the device dynamic configuration schema
   /// \return             The device dynmaic configuration schema
   //--------------------------------------------------------------
   shared::CDataContainer getDynamicConfiguration() const;

   //--------------------------------------------------------------
   /// \brief	    Change the device configuration
   /// \param[in] newConfiguration  The new configuration
   //--------------------------------------------------------------
   void setConfiguration(const shared::CDataContainer& newConfiguration);

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with the counter
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CCounter> m_counter;

   //--------------------------------------------------------------
   /// \brief	The internal counter
   //--------------------------------------------------------------
   unsigned int m_internalCounter;

   //--------------------------------------------------------------
   /// \brief	The divider (set by configuration)
   //--------------------------------------------------------------
   unsigned int m_divider;

   //--------------------------------------------------------------
   /// \brief	Historizers vector
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> > m_historizers;
};

