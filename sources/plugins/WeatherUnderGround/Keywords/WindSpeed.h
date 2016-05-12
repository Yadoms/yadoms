#pragma once
#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Wind Keyword
//--------------------------------------------------------------
class CWindSpeed : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                      Constructor
   /// \param[in] PluginName           The name of the plugin
   /// \param[in] Name                 The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CWindSpeed(std::string PluginName,
              std::string Name);

   //--------------------------------------------------------------
   /// \brief	                         Set the value from the container
   /// \param[in] ValueContainer          The container where the value is stored
   /// \param[in] filterWindDirection     The name of the first information into the container
   /// \note                              Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   void SetValue(const shared::CDataContainer& ValueContainer,
                 const std::string& filterWindDirection) const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CWindSpeed();

   // IKeyword implementation
   void Initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                   shared::CDataContainer details) const override;
   boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const override;
   // [END] IKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_PluginName;

   //--------------------------------------------------------------
   /// \brief	The wind speed
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSpeed> m_windSpeed;
};

