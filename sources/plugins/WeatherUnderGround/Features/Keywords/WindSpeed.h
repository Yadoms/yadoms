#pragma once
#include "IKeyword.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Wind Keyword
//--------------------------------------------------------------
class CWindSpeed : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                      Constructor
   /// \param[in] pluginName           The name of the plugin
   /// \param[in] name                 The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CWindSpeed(const std::string& pluginName,
              const std::string& name);

   //--------------------------------------------------------------
   /// \brief	                         Set the value from the container
   /// \param[in] valueContainer          The container where the value is stored
   /// \param[in] filterWindDirection     The name of the first information into the container
   /// \note                              Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   void setValue(const shared::CDataContainer& valueContainer,
                 const std::string& filterWindDirection) const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CWindSpeed();

   // IKeyword implementation
   boost::shared_ptr<yApi::historization::IHistorizable> getHistorizable() const override;
   // [END] IKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_pluginName;

   //--------------------------------------------------------------
   /// \brief	The wind speed
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSpeed> m_windSpeed;
};

