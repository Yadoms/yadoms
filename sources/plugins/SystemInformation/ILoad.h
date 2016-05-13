#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Interface Load definition
/// \note   return the load in percentage of a value
//--------------------------------------------------------------
class ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ILoad() {}

   //--------------------------------------------------------------
   /// \brief	            Declare the keywords
   /// \param[in] api      yPluginApi context
   /// \param[in] details  more information for the keyword
   //--------------------------------------------------------------
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, 
	                             shared::CDataContainer details) = 0;

   //--------------------------------------------------------------
   /// \brief	    Read actual value
   //--------------------------------------------------------------
   virtual void read() = 0;

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] api      yPluginApi context to which historize data
   //--------------------------------------------------------------
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const = 0;

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \return              The keyword
   //--------------------------------------------------------------
   virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const = 0;
};

