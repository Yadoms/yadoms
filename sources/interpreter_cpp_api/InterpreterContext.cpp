#include "stdafx.h"
#include "InterpreterContext.h"
#include "ApiImplementation.h"
#include "CommandLine.h"
#include "LogConfiguration.h"
#include <shared/currentTime/Local.h>
#include <Poco/Debugger.h>
#include <shared/Log.h>
#include <shared/communication/SmallHeaderMessageAssembler.h>
#include <shared/currentTime/Provider.h>


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

   void CInterpreterContext::run()
   {
      auto api = boost::make_shared<CApiImplementation>();

      try
      {
         openMessageQueues();

         api->setSendingMessageQueue(m_sendMessageQueue);

         m_msgReceiverThread = boost::thread(&CInterpreterContext::msgReceiverThreaded, this, api);

         api->waitInitialized();

         std::cout << api->getInformation()->getType() << " starting" << std::endl;

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
      m_msgReceiverThread.timed_join(boost::posix_time::seconds(30));

      closeMessageQueues();
   }

   void CInterpreterContext::waitDebugger(boost::shared_ptr<CApiImplementation> api) const
   {
      if (boost::filesystem::exists(api->getInformation()->getPath() / "waitForDebuggerAtStart"))
      {
         std::cout << "***********************************************" << std::endl;
         std::cout << " Wait for a debugger to attach current process " << std::endl;
         std::cout << "***********************************************" << std::endl;

         // Check every 300 ms, while 20 seconds (must be < at the delay that Yadoms expect an answer to the interpreter availability request, actually fixed at 30s)
         // Don't wait for stop request as it consumes standard event (interpreter init, interpreter available request...)
         const auto endTimePoint = shared::currentTime::Provider().now() + boost::posix_time::seconds(20);
         while (!Poco::Debugger::isAvailable() && shared::currentTime::Provider().now() < endTimePoint)
         {
            boost::this_thread::sleep(boost::posix_time::millisec(300));
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
         CLogConfiguration logConfig;
         logConfig.configure(api->getLogLevel(), path);
      }
      catch (std::exception& e)
      {
         std::cerr << api->getInformation()->getType() << " fail to configure log system : " << e.what() << std::endl;
      }
      catch (...)
      {
         std::cerr << api->getInformation()->getType() << " fail to configure log system with unknown exception" << std::endl;
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
         m_receiveMessageQueue = boost::make_shared<boost::interprocess::message_queue
         >(boost::interprocess::open_only, receiveMessageQueueId.c_str());
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
               std::cerr << "Error receiving/processing queue message : " << ex.what() << std::endl;
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
      }
   }
} // namespace interpreter_cpp_api
