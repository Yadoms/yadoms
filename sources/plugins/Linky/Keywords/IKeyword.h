#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The KeyWord Interface interface
//--------------------------------------------------------------
class IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IKeyword()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                      Get the IHistorizable variable
   /// \return                       Historizable object
   //--------------------------------------------------------------
   virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const = 0;
};

