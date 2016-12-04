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
                                      const boost::shared_ptr<std::map<std::string, std::string>>& messages) = 0;

   //--------------------------------------------------------------
   /// \brief	                    Return if all information have been read
   /// \return                      If all information have been updated
   //--------------------------------------------------------------
   virtual bool isInformationUpdated() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Refresh update boolean information
   //--------------------------------------------------------------
   virtual void ResetRefreshTags() = 0;

   //--------------------------------------------------------------
   /// \brief	                     Return if the ERDF counter TeleInfo is desactivated
   /// \return                      the counter state
   //--------------------------------------------------------------
   virtual bool isERDFCounterDesactivated() const = 0;
};
