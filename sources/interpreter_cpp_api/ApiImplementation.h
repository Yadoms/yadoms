#pragma once
#include <shared/script/yInterpreterApi/IYInterpreterApi.h>
#include <shared/script/yInterpreterApi/IInformation.h>
#include <toInterpreter.pb.h>
#include <toYadoms.pb.h>


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
      virtual ~CApiImplementation();

      void setSendingMessageQueue(boost::shared_ptr<boost::interprocess::message_queue> sendMessageQueue);

      bool stopRequested() const;

      // shared::plugin::yInterpreterApi::IYInterpreterApi implementation
      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> getInformation() const override;
      shared::event::CEventHandler& getEventHandler() override;
      const boost::filesystem::path& getDataPath() const override;
      // [END] shared::plugin::yInterpreterApi::IYInterpreterApi implementation

      void onReceive(boost::shared_ptr<const unsigned char[]> message, size_t messageSize);

      void waitInitialized() const;

   protected:
      //--------------------------------------------------------------
      /// \brief	Send a request
      /// \param[in] request Request to send
      //--------------------------------------------------------------
      void send(const interpreter_IPC::toYadoms::msg& msg) const;
      void send(const interpreter_IPC::toYadoms::msg& msg,
                boost::function1<bool, const interpreter_IPC::toInterpreter::msg&> checkExpectedMessageFunction,
                boost::function1<void, const interpreter_IPC::toInterpreter::msg&> onReceiveFunction) const;

      void processInit(const interpreter_IPC::toInterpreter::Init& msg);
      void processSystem(const interpreter_IPC::toInterpreter::System& msg);
      void processAvalaibleRequest(const interpreter_IPC::toInterpreter::AvalaibleRequest& msg);
      void processLoadScriptContentRequest(const interpreter_IPC::toInterpreter::LoadScriptContentRequest& msg);
      void processSaveScriptContent(const interpreter_IPC::toInterpreter::SaveScriptContent& msg);
      void processStartScriptRequest(const interpreter_IPC::toInterpreter::StartScriptRequest& msg);
      void processStopScriptRequest(const interpreter_IPC::toInterpreter::StopScriptRequest& msg);

      void setInitialized();

   private:
      mutable std::condition_variable m_initializationCondition;
      mutable std::mutex m_initializationConditionMutex;
      bool m_initialized;

      bool m_stopRequested;

      shared::event::CEventHandler m_pluginEventHandler;


      // The message queue buffer, localy used but defined here to be allocated only once
      boost::shared_ptr<unsigned char[]> m_mqBuffer;

      // The send mutex. Protect m_mqBuffer and m_sendMessageQueue
      mutable boost::recursive_mutex m_sendMutex;

      boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;

      mutable boost::recursive_mutex m_onReceiveHookMutex;
      mutable boost::function1<bool, const interpreter_IPC::toInterpreter::msg&> m_onReceiveHook;

      boost::shared_ptr<shared::script::yInterpreterApi::IInformation> m_pluginInformation;
      boost::shared_ptr<const boost::filesystem::path> m_dataPath;
   };
} // namespace interpreter_cpp_api	


