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
   /// \param [in] data             Data received
   //--------------------------------------------------------------
   virtual void decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const shared::communication::CByteBuffer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	                    Return if all information have been read
   /// \return                      If all information have been updated
   //--------------------------------------------------------------
   virtual bool IsInformationUpdated() = 0;

   //--------------------------------------------------------------
   /// \brief	                     Refresh update boolean information
   //--------------------------------------------------------------
   virtual void ResetRefreshTags() = 0;
};

