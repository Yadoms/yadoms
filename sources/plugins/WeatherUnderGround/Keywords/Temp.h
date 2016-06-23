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
   /// \param[in] PluginName           The name of the plugin
   /// \param[in] KeyWordName          The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CTemp(std::string PluginName,
         std::string KeyWordName);

   //--------------------------------------------------------------
   /// \brief	                      Set the value from the container
   /// \param[in] ValueContainer       The container where the value is stored
   /// \param[in] filter               The name of the information into the container
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   void SetValue(const shared::CDataContainer& ValueContainer,
                 const std::string& filter) const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CTemp();

   // IKeyword implementation
   void Initialize(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details) const override;
   boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const override;
   // [END] IKeyword implementation

private:

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_PluginName;

   //--------------------------------------------------------------
   /// \brief	The temperature (°C)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
};

