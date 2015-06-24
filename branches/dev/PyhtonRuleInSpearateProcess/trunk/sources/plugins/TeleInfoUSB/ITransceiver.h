#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ITeleInfoConfiguration.h"
#include <shared/communication/Buffer.hpp>
#include "ITeleInfoMessage.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The TeleInfo protocol interface
//--------------------------------------------------------------
class ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ITransceiver() {}

   //--------------------------------------------------------------
   /// \brief	                     Decode TeleInfo Reader message
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   //--------------------------------------------------------------
   virtual void decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> context, 
	                                  std::string & PluginName,
                                      const shared::communication::CByteBuffer& data) = 0;
};
