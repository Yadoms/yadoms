#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/communication/Buffer.hpp>

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
   virtual ~ITransceiver()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                    Decode TeleInfo Reader message
   /// \param [in] api             Plugin execution context (Yadoms API)
   /// \param [in] messages        Received messages
   //--------------------------------------------------------------
   virtual void decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const boost::shared_ptr<std::map<std::string, std::string>>& messages) = 0;
   
   //--------------------------------------------------------------
   /// \brief	                     Return if the ERDF counter TeleInfo is desactivated
   /// \return                      the counter state
   //--------------------------------------------------------------
   virtual bool isERDFCounterDesactivated() const = 0;
};
