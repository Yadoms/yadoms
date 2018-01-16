#pragma once

#include "IKeyword.h"
#include "../specificHistorizer/Period.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Load class
//--------------------------------------------------------------
class CRunningPeriod : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                      Constructor
   /// \param[in] api                Yadoms api
   /// \param[in] KeyWordName        The keyword name
   /// \note                         Use this constructor initializing the keyword for this plugin
   //--------------------------------------------------------------
   explicit CRunningPeriod(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName);

   //--------------------------------------------------------------
   /// \brief	                      Set the value of the keyword
   /// \param[in] value              The new value
   //--------------------------------------------------------------
   void set(const std::string& value);

   //--------------------------------------------------------------
   /// \brief	                      Get if the value have changed
   /// \return                       true, if it's a new value
   //--------------------------------------------------------------
   bool isChanged() const;

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRunningPeriod();

   // IKeyword implementation
   boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const override;
   // [END] IKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_PluginName;

   //--------------------------------------------------------------
   /// \brief	The period type
   //--------------------------------------------------------------
   boost::shared_ptr<teleInfo::specificHistorizers::CPeriod> m_runningPeriod;

   //--------------------------------------------------------------
   /// \brief	The map of returned value to the IHM
   //--------------------------------------------------------------
   typedef std::map<std::string, unsigned int> EnumPeriod;

   //--------------------------------------------------------------
   /// \brief	developer mode
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

   //--------------------------------------------------------------
   /// \brief	if the value change
   //--------------------------------------------------------------
   bool m_isChanged;
};