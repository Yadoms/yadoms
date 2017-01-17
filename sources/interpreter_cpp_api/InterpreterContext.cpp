#include "stdafx.h"
#include "InterpreterContext.h"
#include "ApiImplementation.h"
#include "CommandLine.h"
#include <shared/currentTime/Local.h>
#include <Poco/Debugger.h>
#include <shared/Log.h>
#include <shared/process/YadomsSubModuleLogConfiguration.h>


namespace yApi = shared::script::yInterpreterApi;

namespace interpreter_cpp_api
{
   CInterpreterContext::CInterpreterContext(int argc,
                                            char** argv,
                                            boost::shared_ptr<IInterpreter> interpreter)
      : m_commandLine(boost::make_shared<CCommandLine>(argc, argv)),
        m_interpreter(interpreter),
        m_returnCode(kOk)
   {
      shared::currentTime::Provider().setProvider(boost::make_shared<shared::currentTime::Local>());
   }

   CInterpreterContext::~CInterpreterContext()
   {
   }

   void CInterpreterContext::run()
   {
      auto api = boost::make_shared<CApiImplementation>();

      try
      {
         openMessageQueues();

         api->setSendingMessageQueue(m_sendMessageQueue);

         m_msgReceiverThread = boost::thread(&CInterpreterContext::msgReceiverThreaded, this, api);

         api->waitInitialized();

         std::cout << api->getInformation()->getType() << " started" << std::endl;

         waitDebugger(api);

         configureLogger(api);

         YADOMS_LOG(information) << api->getInformation()->getType() << " started";

         if (!api->stopRequested())
         {
            // Execute interpreter code
            m_interpreter->doWork(api);
         }

         if (!api->stopRequested())
         {
            std::cerr << api->getInformation()->getType() << " has stopped itself." << std::endl;
            m_returnCode = kUnexpectedStop;
         }

         // Normal stop
         std::cout << api->getInformation()->getType() << " is stopped" << std::endl;
         m_returnCode = kOk;
      }
      catch (std::exception& e)
      {
         std::cerr << api->getInformation()->getType() << " crashed with exception : " << e.what();
         m_returnCode = kRuntimeError;
      }
      catch (...)
      {
         std::cerr << api->getInformation()->getType() << " crashed with unknown exception" << std::endl;
         m_returnCode = kRuntimeError;
      }

      m_msgReceiverThread.interrupt();
      m_msgReceiverThread.timed_join(boost::posix_time::seconds(20));

      closeMessageQueues();
   }

   void CInterpreterContext::waitDebugger(boost::shared_ptr<CApiImplementation> api) const
   {
      if (boost::filesystem::exists(api->getInformation()->getPath() / "waitForDebuggerAtStart"))
      {
         std::cout << "***********************************************" << std::endl;
         std::cout << " Wait for a debugger to attach current process " << std::endl;
         std::cout << "***********************************************" << std::endl;

         // Check every 300 ms, while 2 minutes
         const auto endTimePoint = shared::currentTime::Provider().now() + boost::posix_time::minutes(2);
         while (!Poco::Debugger::isAvailable() && shared::currentTime::Provider().now() < endTimePoint)
         {
            if (api->getEventHandler().waitForEvents(boost::posix_time::millisec(300)) == yApi::IYInterpreterApi::kEventStopRequested)
            {
               std::cout << "Stop requested" << std::endl;
               return;
            }
         }

         if (Poco::Debugger::isAvailable())
            std::cout << api->getInformation()->getType() << " attached to debugger" << std::endl;
         else
            std::cout << api->getInformation()->getType() << " failed to attach debugger after timeout" << std::endl;
      }
   }

   void CInterpreterContext::configureLogger(boost::shared_ptr<CApiImplementation> api)
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
         std::cerr << api->getInformation()->getType() << " fail to confiugure log system : " << e.what() << std::endl;
      }
      catch (...)
      {
         std::cerr << api->getInformation()->getType() << " fail to confiugure log system with unknown exception" << std::endl;
      }

      YADOMS_LOG_CONFIGURE(api->getInformation()->getType());
   }

   IInterpreterContext::EProcessReturnCode CInterpreterContext::getReturnCode() const
   {
      return m_returnCode;
   }

   void CInterpreterContext::openMessageQueues() const
   {
      // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

      try
      {
         const auto sendMessageQueueId(m_commandLine->yInterpreterApiAccessorId() + ".interpreter_IPC.toYadoms");
         const auto receiveMessageQueueId(m_commandLine->yInterpreterApiAccessorId() + ".interpreter_IPC.toInterpreter");

         std::cout << "Opening message queues id " << m_commandLine->yInterpreterApiAccessorId() << std::endl;

         m_sendMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, sendMessageQueueId.c_str());
         m_receiveMessageQueue = boost::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, receiveMessageQueueId.c_str());
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         throw std::runtime_error(std::string("CInterpreterContext::msgReceiverThreaded : Error opening message queue, ") + ex.what());
      }
      catch (...)
      {
         throw std::runtime_error("CInterpreterContext::msgReceiverThreaded : Unknown error");
      }
   }

   void CInterpreterContext::closeMessageQueues()
   {
      std::cout << "Close message queues" << std::endl;

      // Delete all global objects allocated by libprotobuf.
      google::protobuf::ShutdownProtobufLibrary();
   }

   void CInterpreterContext::msgReceiverThreaded(boost::shared_ptr<CApiImplementation> api) const
   {
      auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue->get_max_msg_size()));
      size_t messageSize;
      unsigned int messagePriority;

      try
      {
         while (true)
         {
            try
            {
               // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
               // polling and call boost::this_thread::interruption_point to exit properly
               // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
               auto messageWasReceived = m_receiveMessageQueue->timed_receive(message.get(),
                                                                              m_receiveMessageQueue->get_max_msg_size(),
                                                                              messageSize,
                                                                              messagePriority,
                                                                              boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1));
               boost::this_thread::interruption_point();

               if (messageWasReceived)
                  api->onReceive(message, messageSize);
            }
            catch (shared::exception::CInvalidParameter& ex)
            {
               std::cerr << "Error receiving/processing queue message : " << ex.what() << std::endl;
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
      }
   }
} // namespace interpreter_cpp_api


