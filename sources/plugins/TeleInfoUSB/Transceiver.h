#pragma once

#include "ITransceiver.h"
#include <shared/communication/IAsyncPort.h>
#include "ITeleInfoMessage.h"
#include "TeleInfotrxHelpers.h"
#include "ISequenceNumberProvider.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the TeleInfo protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   CTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> context,
	                                  std::string & PluginName,
                                      const shared::communication::CByteBuffer& data) const;
   // [END] ITransceiver implementation
   
private:
   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;

   bool isCheckSumOk(const unsigned char *m_buffer) const;

   void ParseData(boost::shared_ptr<yApi::IYPluginApi> context, std::string & PluginName, const unsigned char *pData, int Len) const;

   void MatchLine( const unsigned char*m_buffer, boost::shared_ptr<yApi::IYPluginApi> context, std::string & PluginName ) const;
};