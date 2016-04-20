#pragma once

#include "IPluginContext.h"
#include "IPlugin.h"
#include "ICommandLine.h"

// TODO faire un include dans plugin_IPC qui contient ces 3 lignes
#include "ApiImplementation.h"
const size_t m_messageQueueMessageSize(1000); //TODO déplacer


namespace plugin_cpp_api
{
   //--------------------------------------------------------------
   /// \class The plugin context
   //--------------------------------------------------------------
   class CPluginContext : public IPluginContext
   {
   public:
      //TODO commenter
      CPluginContext(int argc, char **argv, boost::shared_ptr<IPlugin> plugin);

      virtual ~CPluginContext();

      void run() override;

      EReturnCode getReturnCode() const override;

   protected:
      void openMessageQueues() const;
      static void closeMessageQueues();

   private:
      boost::shared_ptr<ICommandLine> m_commandLine;
      boost::shared_ptr<IPlugin> m_plugin;

      EReturnCode m_returnCode;

      boost::thread m_msgReceiverThread;
      void msgReceiverThreaded(boost::shared_ptr<CApiImplementation> api) const;

      //-----------------------------------------------------
      ///\brief               The message queues used to exchange data with Yadoms
      //\details These objects must be mutable to preserve IYScriptApi const interface
      //-----------------------------------------------------
      mutable boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;
      mutable boost::shared_ptr<boost::interprocess::message_queue> m_receiveMessageQueue;
   };

} // namespace plugin_cpp_api
