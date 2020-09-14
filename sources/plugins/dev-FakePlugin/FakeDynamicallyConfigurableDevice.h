#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Fake configurable device
/// \note   User can configure the counter divider (increment each N read operations)
//--------------------------------------------------------------
class CFakeDynamicallyConfigurableDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   /// \param[in] configuration The device configuration
   //--------------------------------------------------------------
   explicit CFakeDynamicallyConfigurableDevice(const std::string& deviceName,
                                               const boost::shared_ptr<shared::CDataContainer>& configuration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeDynamicallyConfigurableDevice() = default;

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
   /// \brief	    Returns the fake sensor type
   /// \return     The fake sensor type
   //--------------------------------------------------------------
   static const std::string& getType();

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The fake sensor model
   //--------------------------------------------------------------
   static const std::string& getModel();

   //--------------------------------------------------------------
   /// \brief	    Change the device configuration
   /// \param[in] newConfiguration  The new configuration
   //--------------------------------------------------------------
   void setConfiguration(const boost::shared_ptr<shared::CDataContainer>& newConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Get the historizers
   /// \return     The device historizers
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> historizers() const;

   //--------------------------------------------------------------
   /// \brief	            Get the device dynamic configuration schema
   /// \return             The device dynamic configuration schema
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::CDataContainer> getDynamicConfigurationSchema();

protected:
   static double readDividerConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration);


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
   double m_divider;

   //--------------------------------------------------------------
   /// \brief	Historizers vector
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizers;
};

