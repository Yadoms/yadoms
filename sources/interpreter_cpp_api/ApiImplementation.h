#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>
#include <shared/script/yInterpreterApi/IInformation.h>
#include <interpreter_IPC/yadomsToInterpreter.pb.h>
#include <interpreter_IPC/interpreterToYadoms.pb.h>
#include <shared/communication/IMessageCutter.h>


namespace interpreter_cpp_api
{
   //-----------------------------------------------------
   ///\brief The Yadoms plugin API implementation for the CPP plugin API
   //-----------------------------------------------------
   class CApiImplementation : public shared::script::yInterpreterApi::IYInterpreterApi
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CApiImplementation();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CApiImplementation() = default;

      void setSendingMessageQueue(boost::shared_ptr<boost::interprocess::message_queue> sendMessageQueue);

      bool stopRequested() const;

      // shared::plugin::yInterpreterApi::IYInterpreterApi implementation
      void notifyScriptStopped(int scriptInstanceId,
                               const std::string& error) override;
      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> getInformation() const override;
      shared::event::CEventHandler& getEventHandler() override;
      // [END] shared::plugin::yInterpreterApi::IYInterpreterApi implementation

      const boost::filesystem::path& getLogFile() const;
      const std::string& getLogLevel() const;

      void onReceive(boost::shared_ptr<const unsigned char[]> message, size_t messageSize);

      void waitInitialized() const;

   protected:
      //--------------------------------------------------------------
      /// \brief	Send a request
      //--------------------------------------------------------------
      void send(const interpreter_IPC::toYadoms::msg& msg) const;
      void send(const interpreter_IPC::toYadoms::msg& msg,
                boost::function1<bool, const interpreter_IPC::toInterpreter::msg&> checkExpectedMessageFunction,
                boost::function1<void, const interpreter_IPC::toInterpreter::msg&> onReceiveFunction) const;

      void processInit(const interpreter_IPC::toInterpreter::Init& msg);
      void processSystem(const interpreter_IPC::toInterpreter::System& msg);
      void processAvailableRequest(const interpreter_IPC::toInterpreter::AvalaibleRequest& msg);
      void processLoadScriptContentRequest(const interpreter_IPC::toInterpreter::LoadScriptContentRequest& msg);
      void processSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContentRequest& msg);
      void processStartScript(const interpreter_IPC::toInterpreter::StartScript& msg);
      void processStopScript(const interpreter_IPC::toInterpreter::StopScript& msg);
      void processPurgeScriptLog(const interpreter_IPC::toInterpreter::PurgeScriptLog& msg);

      void setInitialized();

   private:
      mutable std::condition_variable m_initializationCondition;
      mutable std::mutex m_initializationConditionMutex;
      bool m_initialized;

      bool m_stopRequested;

      shared::event::CEventHandler m_interpreterEventHandler;


      // The buffer
      boost::shared_ptr<shared::communication::IMessageCutter> m_messageCutter;

      // The send mutex. Protect m_mqBuffer and m_sendMessageQueue
      mutable boost::recursive_mutex m_sendMutex;

      boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;

      mutable boost::recursive_mutex m_onReceiveHookMutex;
      mutable boost::function1<bool, const interpreter_IPC::toInterpreter::msg&> m_onReceiveHook;

      boost::shared_ptr<shared::script::yInterpreterApi::IInformation> m_interpreterInformation;
      boost::shared_ptr<const boost::filesystem::path> m_logFile;
      boost::shared_ptr<const std::string> m_logLevel;
   };
} // namespace interpreter_cpp_api	
