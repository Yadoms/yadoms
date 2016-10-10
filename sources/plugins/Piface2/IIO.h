#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Interface IO definition
//--------------------------------------------------------------
class IIO
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IIO()
   {
   }

   //--------------------------------------------------------------
   /// \brief	            
   /// \return              The keyword
   //--------------------------------------------------------------
   virtual boost::shared_ptr<const yApi::historization::IHistorizable> historizable() const = 0;
};

