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
   /// \param[in] context  yPluginApi context
   //--------------------------------------------------------------
   virtual void declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context) = 0;

   //--------------------------------------------------------------
   /// \brief	    Read actual value
   //--------------------------------------------------------------
   virtual void read() = 0;

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] context  yPluginApi context to which historize data
   //--------------------------------------------------------------
   virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const = 0;
};

