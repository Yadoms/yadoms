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
   virtual ~ILoad()
   {
   }

   //--------------------------------------------------------------
   /// \brief	    Read actual value
   //--------------------------------------------------------------
   virtual void read() = 0;

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \return              The keyword
   //--------------------------------------------------------------
   virtual boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const = 0;
};

