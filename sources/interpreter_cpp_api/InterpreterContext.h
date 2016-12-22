#pragma once
#include "IInterpreterContext.h"
#include "interpreter_cpp_api/IInterpreter.h"
#include "ICommandLine.h"
#include "ApiImplementation.h"

namespace interpreter_cpp_api
{
   //--------------------------------------------------------------
   /// \class The interpreter context
   //--------------------------------------------------------------
   class CInterpreterContext : public IInterpreterContext
   {
   public:
      //--------------------------------------------------------------
      ///\brief               Constructor
      ///\param[in] argc      Command line arguments number
      ///\param[in] argv      Command line arguments list
      ///\param[in] interpreter    Main interpreter object
      //--------------------------------------------------------------
      CInterpreterContext(int argc,
                     char** argv,
                     boost::shared_ptr<IInterpreter> interpreter);

      //--------------------------------------------------------------
      ///\brief               Destructor
      //--------------------------------------------------------------
      virtual ~CInterpreterContext();

      // IInterpreterContext Implementation
      void run() override;
      EProcessReturnCode getReturnCode() const override;
      // [END] IInterpreterContext Implementation

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

   private:
      //--------------------------------------------------------------
      ///\brief               The command line used to start the interpreter
      //--------------------------------------------------------------
      boost::shared_ptr<ICommandLine> m_commandLine;

      //--------------------------------------------------------------
      ///\brief               The main interpreter object
      //--------------------------------------------------------------
      boost::shared_ptr<IInterpreter> m_plugin;

      //--------------------------------------------------------------
      ///\brief               The interpreter process exit code
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
} // namespace interpreter_cpp_api


