#pragma once

#include "IKeyword.h"
#include "../specificHistorizer/Color.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Load class
//--------------------------------------------------------------
class CForecastTomorrow : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                      Constructor
   /// \param[in] api                Yadoms api
   /// \param[in] KeyWordName        The keyword name
   /// \note                         Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   explicit CForecastTomorrow(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName);

   //--------------------------------------------------------------
   /// \brief	                      Set the value of the keyword
   /// \param[in] value                The new value
   //--------------------------------------------------------------
   void set(const linky::specificHistorizers::EColor value) const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CForecastTomorrow();

   // IKeyword implementation
   boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const override;
   // [END] IKeyword implementation

private:

   //--------------------------------------------------------------
   /// \brief	The period type
   //--------------------------------------------------------------
   boost::shared_ptr<linky::specificHistorizers::CColor> m_forecastPeriod;

   //--------------------------------------------------------------
   /// \brief	The map of returned value to the IHM
   //--------------------------------------------------------------
   typedef std::map<std::string, unsigned int> EnumColorMap;

   //--------------------------------------------------------------
   /// \brief	developer mode
   //--------------------------------------------------------------
   bool m_isDeveloperMode;
};

