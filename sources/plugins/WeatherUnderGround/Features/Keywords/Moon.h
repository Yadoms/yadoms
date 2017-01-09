#pragma once

#include "IKeyword.h"
#include "specificHistorizers/MoonHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Forecast class
//--------------------------------------------------------------
class CMoon : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in] pluginName           The name of the plugin
   /// \param[in] keyWordName          The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CMoon(const std::string& pluginName,
         const std::string& keyWordName);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CMoon();

   //-----------------------------------------------------
   ///\brief                      Add a new unit to be send to the widget
   ///\param[in] unitName         The Unit Name
   ///\param[in] unitValue        The Unit Value
   //-----------------------------------------------------
   void addUnit(const std::string& unitName,
                const std::string& unitValue) const;

   //-----------------------------------------------------
   ///\brief                      Add a new day with all integrated parameter
   ///\param[in] illuminatedMoon  Moon illumination
   ///\param[in] dayofMoon        Moon day
   ///\throw                      shared::exception::CInvalidParameter or COutOfRange if fail to parse command
   //-----------------------------------------------------
   void setParameters(const shared::CDataContainer& valueContainer,
                      const std::string& illuminatedMoon,
                      const std::string& dayofMoon) const;

   // IKeyword implementation
   boost::shared_ptr<yApi::historization::IHistorizable> getHistorizable() const override;
   // [END] IKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_pluginName;

   //--------------------------------------------------------------
   /// \brief	The pressure (hPa)
   //--------------------------------------------------------------
   boost::shared_ptr<specificHistorizer::CMoonHistorizer> m_moonCharacteristics;
};

