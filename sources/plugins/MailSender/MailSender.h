#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "IMSConfiguration.h"
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/SharedPtr.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the MailSender plugin
/// \note   This plugin send a mail when receive a notification
//--------------------------------------------------------------
class CMailSender : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CMailSender();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CMailSender();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief Get the e-mail of the received recipient
   /// \param[in] api                   pointer to the API
   /// \param[in] recipientId           recipientId
   //--------------------------------------------------------------
   std::string getRecipientMail(boost::shared_ptr<yApi::IYPluginApi> api,
                                int recipientId) const;

   //--------------------------------------------------------------
   /// \brief Send a mail
   /// \param[in] api                   pointer to the API
   /// \param[in] sendMailRequest       Struture of the sending mail
   //--------------------------------------------------------------
   void onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& sendMailRequest);

   //--------------------------------------------------------------
   /// \brief Declare the device and all keywords associated
   /// \param[in] api                  pointer to the API
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainerSharedPtr& newConfigurationData) const;

private:
   //--------------------------------------------------------------
   /// \brief The device name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<IMSConfiguration> m_configuration;

   //--------------------------------------------------------------
   /// \brief	    Recipient field used to retrieve e-mail from a recipient
   //--------------------------------------------------------------
   std::string m_mailId;

   //--------------------------------------------------------------
   /// \brief	    Message historization object
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CMessage> m_messageKeyword;

   //--------------------------------------------------------------
   /// \brief	    The ceertificate passphrase provider
   //--------------------------------------------------------------
   Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> m_certificatePassphraseProvider;
};

