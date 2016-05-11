#pragma once
#include <plugin_cpp_api/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	FreeMobile plugin. Allow to send notification through FreeMobileApi
//--------------------------------------------------------------
class CFreeMobileSms : public plugin_cpp_api::IPlugin
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
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	Send a sms to a recipient
   /// \param [in]   api            The plugin context
   /// \param [in]   recipientId    The recipient id to send sms to
   /// \param [in]   smsContent     The SMS content
   //--------------------------------------------------------------
   static void sendSms(boost::shared_ptr<yApi::IYPluginApi> api,
                       const int recipientId,
                       const std::string& smsContent);


   static std::string m_freeMobileApiUrl;
   static std::string m_deviceName;
   static std::string m_keywordName;
};

