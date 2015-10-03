#pragma once
#include <shared/plugin/IPlugin.h>
#include "MSConfiguration.h"
#include <quickmail.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the MailSender plugin
/// \note   This plugin send a mail when receive a notification
//--------------------------------------------------------------
class CMailSender : public shared::plugin::IPlugin
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
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Get the e-mail of the received recipient
   /// \param[in] context               pointer to the API
   /// \param[in] recipientId           recipientId
   //--------------------------------------------------------------
   std::string getRecipientMail(boost::shared_ptr<yApi::IYPluginApi> context, int recipientId);

   //--------------------------------------------------------------
   /// \brief Send a mail
   /// \param[in] context               pointer to the API
   /// \param[in] sendMailRequest       Struture of the sending mail
   //--------------------------------------------------------------
   void onSendMailRequest(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& sendMailRequest);

   //--------------------------------------------------------------
   /// \brief Declare the device and all keywords associated
   /// \param[in] context               pointer to the API
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> context);

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] context               pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData);

private:
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CMSConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	    Recipient field used to retrieve e-mail from a recipient
   //--------------------------------------------------------------
   std::string m_mailId;

   //--------------------------------------------------------------
   /// \brief	    Message historization object
   //--------------------------------------------------------------
   yApi::historization::CMessage m_messageKeyword;

   //--------------------------------------------------------------
   /// \brief	    Create the structure of the mail
   //--------------------------------------------------------------
   quickmail mailobj;
};
