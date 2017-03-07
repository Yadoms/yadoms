#pragma once
#include "IPluginContext.h"
#include "plugin_cpp_api/IPlugin.h"
#include "ICommandLine.h"
#include "ApiImplementation.h"

namespace plugin_cpp_api
{
   //--------------------------------------------------------------
   /// \class The plugin context
   //--------------------------------------------------------------
   class CPluginContext : public IPluginContext
   {
   public:
      //--------------------------------------------------------------
      ///\brief               Constructor
      ///\param[in] argc      Command line arguments number
      ///\param[in] argv      Command line arguments list
      ///\param[in] plugin    Main plugin object
      //--------------------------------------------------------------
      CPluginContext(int argc,
                     char** argv,
                     boost::shared_ptr<IPlugin> plugin);

      //--------------------------------------------------------------
      ///\brief               Destructor
      //--------------------------------------------------------------
      virtual ~CPluginContext();

      // IPluginContext Implementation
      void run() override;
      EProcessReturnCode getReturnCode() const override;
      // [END] IPluginContext Implementation

   protected:
      //--------------------------------------------------------------
      ///\brief               Open the message queues used to dial with Yadoms
      //--------------------------------------------------------------
      void openMessageQueues() const;

      //--------------------------------------------------------------
      ///\brief               Close the message queues
      //--------------------------------------------------------------
      static void closeMessageQueues();

      //--------------------------------------------------------------
      ///\brief               Wait for debugger (debug only)
      //--------------------------------------------------------------
      void waitDebugger(boost::shared_ptr<CApiImplementation> api) const;

      //--------------------------------------------------------------
      ///\brief               Configure the logger
      //--------------------------------------------------------------
      static void configureLogger(boost::shared_ptr<CApiImplementation> api);

   private:
      //--------------------------------------------------------------
      ///\brief               The command line used to start the plugin
      //--------------------------------------------------------------
      boost::shared_ptr<ICommandLine> m_commandLine;

      //--------------------------------------------------------------
      ///\brief               The main plugin object
      //--------------------------------------------------------------
      boost::shared_ptr<IPlugin> m_plugin;

      //--------------------------------------------------------------
      ///\brief               The plugin process exit code
      //--------------------------------------------------------------
      EProcessReturnCode m_returnCode;

      //--------------------------------------------------------------
      ///\brief               The thread used to receive messages from Yadoms
      //--------------------------------------------------------------
      boost::thread m_msgReceiverThread;

      //--------------------------------------------------------------
      ///\brief               The thread function to receive messages from Yadoms
      //--------------------------------------------------------------
      void msgReceiverThreaded(boost::shared_ptr<CApiImplementation> api) const;

      //-----------------------------------------------------
      ///\brief               The message queues used to exchange data with Yadoms
      //\details These objects must be mutable to preserve IYScriptApi const interface
      //-----------------------------------------------------
      mutable boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;
      mutable boost::shared_ptr<boost::interprocess::message_queue> m_receiveMessageQueue;
   };
} // namespace plugin_cpp_api


