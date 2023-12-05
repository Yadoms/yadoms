#include "stdafx.h"
#include "PluginContext.h"
#include "ApiImplementation.h"
#include "CommandLine.h"
#include <shared/currentTime/Local.h>
#include <Poco/Debugger.h>
#include <shared/Log.h>
#include <shared/process/YadomsSubModuleLogConfiguration.h>
#include <shared/communication/SmallHeaderMessageAssembler.h>

namespace yApi = shared::plugin::yPluginApi;

namespace plugin_cpp_api
{
   CPluginContext::CPluginContext(int argc,
                                  char** argv,
                                  const boost::shared_ptr<IPlugin>& plugin)
      : m_commandLine(boost::make_shared<CCommandLine>(argc, argv)),
        m_plugin(plugin),
        m_returnCode(kOk)
   {
      shared::currentTime::Provider().setProvider(boost::make_shared<shared::currentTime::Local>());
   }

   void CPluginContext::run()
   {
      const auto api = boost::make_shared<CApiImplementation>();

      try
      {
         openMessageQueues();

         api->setSendingMessageQueue(m_sendMessageQueue);

         m_msgReceiverThread = boost::thread(&CPluginContext::msgReceiverThreaded, this, api);

         api->waitInitialized();

         std::cout << api->getInformation()->getType() << " starting" << std::endl;

         waitDebugger(api);

         configureLogger(api);

         YADOMS_LOG(information) << api->getInformation()->getType() << " started";

         if (!api->stopRequested())
         {
            // Execute plugin code
            m_plugin->doWork(api);
         }

         if (!api->stopRequested())
         {
            YADOMS_LOG(error) << api->getInformation()->getType() << " has stopped itself.";
            m_returnCode = kUnexpectedStop;
         }

         // Normal stop
         YADOMS_LOG(information) << api->getInformation()->getType() << " is stopped";
         m_returnCode = kOk;
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << api->getInformation()->getType() << " crashed with exception : " << e.what();
         m_returnCode = kRuntimeError;
      }
      catch (...)
      {
         YADOMS_LOG(error) << api->getInformation()->getType() << " crashed with unknown exception";
         m_returnCode = kRuntimeError;
      }

      m_msgReceiverThread.interrupt();
      m_msgReceiverThread.timed_join(boost::posix_time::seconds(20));

      closeMessageQueues();
   }

   void CPluginContext::waitDebugger(const boost::shared_ptr<CApiImplementation>& api)
   {
      if (boost::filesystem::exists(api->getInformation()->getPath() / "waitForDebuggerAtStart"))
      {
         api->setPluginState(yApi::historization::EPluginState::kWaitDebugger);
         std::cout << "***********************************************" << std::endl;
         std::cout << " Wait for a debugger to attach current process " << std::endl;
         std::cout << "***********************************************" << std::endl;

         // Check every 300 ms, while 2 minutes
         const auto endTimePoint = shared::currentTime::Provider().now() + boost::posix_time::minutes(2);
         while (!Poco::Debugger::isAvailable() && shared::currentTime::Provider().now() < endTimePoint)
         {
            if (api->getEventHandler().waitForEvents(boost::posix_time::millisec(300)) == yApi::IYPluginApi::kEventStopRequested)
            {
               std::cout << "Stop requested" << std::endl;
               api->setPluginState(yApi::historization::EPluginState::kStopped);
               return;
            }
         }

         if (Poco::Debugger::isAvailable())
            std::cout << api->getInformation()->getType() << " attached to debugger" << std::endl;
         else
            std::cerr << api->getInformation()->getType() << " failed to attach debugger after timeout" << std::endl;
      }
   }

   void CPluginContext::configureLogger(const boost::shared_ptr<CApiImplementation>& api)
   {
      try
      {
         auto path = api->getLogFile();
         std::cout << api->getInformation()->getType() << " configure logger : " << path.string() << std::endl;
         shared::process::CYadomsSubModuleLogConfiguration logconfig;
         logconfig.configure(api->getLogLevel(), path);
      }
      catch (std::exception& e)
      {
         std::cerr << api->getInformation()->getType() << " fail to configure log system : " << e.what() << std::endl;
      }
      catch (...)
      {
         std::cerr << api->getInformation()->getType() << " fail to configure log system with unknown exception" << std::endl;
      }

      YADOMS_LOG_CONFIGURE("mainThread")
   }

   IPluginContext::EProcessReturnCode CPluginContext::getReturnCode() const
   {
      return m_returnCode;
   }

   void CPluginContext::openMessageQueues() const
   {
      // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

      try
      {
         const auto sendMessageQueueId(m_commandLine->yPluginApiAccessorId() + ".plugin_IPC.toYadoms");
         const auto receiveMessageQueueId(m_commandLine->yPluginApiAccessorId() + ".plugin_IPC.toPlugin");

         std::cout << "Opening message queues id " << m_commandLine->yPluginApiAccessorId() << std::endl;

         m_sendMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, sendMessageQueueId.c_str());
         m_receiveMessageQueue = boost::make_shared<boost::interprocess::message_queue
         >(boost::interprocess::open_only, receiveMessageQueueId.c_str());
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         throw std::runtime_error(std::string("CPluginContext::msgReceiverThreaded : Error opening message queue, ") + ex.what());
      }
      catch (...)
      {
         throw std::runtime_error("CPluginContext::msgReceiverThreaded : Unknown error");
      }
   }

   void CPluginContext::closeMessageQueues()
   {
      YADOMS_LOG(information) << "Close message queues";

      // Delete all global objects allocated by libprotobuf.
      google::protobuf::ShutdownProtobufLibrary();
   }

   void CPluginContext::msgReceiverThreaded(const boost::shared_ptr<CApiImplementation>& api) const
   {
      const auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue->get_max_msg_size()));
      const auto messageAssembler = boost::make_shared<shared::communication::SmallHeaderMessageAssembler>(m_receiveMessageQueue->get_max_msg_size());

      try
      {
         while (true)
         {
            try
            {
               size_t messageSize;
               unsigned int messagePriority;

               // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
               // polling and call boost::this_thread::interruption_point to exit properly
               // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
               const auto messageWasReceived = m_receiveMessageQueue->timed_receive(message.get(),
                                                                                    m_receiveMessageQueue->get_max_msg_size(),
                                                                                    messageSize,
                                                                                    messagePriority,
                                                                                    boost::posix_time::microsec_clock::universal_time() + boost::
                                                                                    posix_time::seconds(1));
               boost::this_thread::interruption_point();

               if (messageWasReceived)
               {
                  messageAssembler->appendPart(message,
                                               messageSize);

                  if (messageAssembler->isCompleted())
                  {
                     api->onReceive(messageAssembler->message(),
                                    messageAssembler->messageSize());
                  }
               }
            }
            catch (shared::exception::CInvalidParameter& ex)
            {
               YADOMS_LOG(error) << "Error receiving/processing queue message : " << ex.what();
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
      }
   }
} // namespace plugin_cpp_api
