#pragma once
#include <shared/plugin/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	FreeMobile plugin. Allow to send notification through FreeMobileApi
//--------------------------------------------------------------
class CFreeMobileSms : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CFreeMobileSms();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CFreeMobileSms();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
  // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	Send a sms to a recipient
   /// \param [in]   context        The plugin context
   /// \param [in]   recipientId    The recipient id to send sms to
   /// \param [in]   smsContent     The SMS content
   //--------------------------------------------------------------
   void sendSms(boost::shared_ptr<yApi::IYPluginApi> context, const int recipientId, const std::string & smsContent);
private:
   static std::string m_freeMobileApiUrl;
   static std::string m_deviceName;
   static std::string m_keywordName;
};



