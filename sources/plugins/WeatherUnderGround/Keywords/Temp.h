#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IKeyword.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Temp capacities
//--------------------------------------------------------------
class CTemp : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                      Constructor
   /// \param[in] pluginName           The name of the plugin
   /// \param[in] keyWordName          The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CTemp(std::string pluginName,
         std::string keyWordName);

   //--------------------------------------------------------------
   /// \brief	                      Set the value from the container
   /// \param[in] valueContainer       The container where the value is stored
   /// \param[in] filter               The name of the information into the container
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   void setValue(const shared::CDataContainer& valueContainer,
                 const std::string& filter) const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CTemp();

   // IKeyword implementation
   void initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                   shared::CDataContainer details) const override;
   boost::shared_ptr<yApi::historization::IHistorizable> getHistorizable() const override;
   // [END] IKeyword implementation

private:

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_pluginName;

   //--------------------------------------------------------------
   /// \brief	The temperature (°C)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
};

