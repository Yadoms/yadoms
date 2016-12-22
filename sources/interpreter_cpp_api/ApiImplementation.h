#pragma once
//#include <shared/script/yInterpreterApi/IYInterpreterApi.h> //TODO utile ?
#include <interpreter_IPC/interpreter_IPC.h>


namespace interpreter_cpp_api
{
   //-----------------------------------------------------
   ///\brief The Yadoms plugin API implementation for the CPP plugin API
   //-----------------------------------------------------
   class CApiImplementation /*: public shared::plugin::yInterpreterApi::IYInterpreterApi TODO utile ?*/
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

      void processSystem(const interpreter_IPC::toInterpreter::System& msg);
      void processInit(const interpreter_IPC::toInterpreter::Init& msg);
      void processUpdateConfiguration(const interpreter_IPC::toInterpreter::Configuration& msg);
      void processBindingQuery(const interpreter_IPC::toInterpreter::BindingQuery& msg);
      void processDeviceConfigurationSchemaRequest(const interpreter_IPC::toInterpreter::DeviceConfigurationSchemaRequest& msg);
      void processSetDeviceConfiguration(const interpreter_IPC::toInterpreter::SetDeviceConfiguration& msg);
      void processDeviceCommand(const interpreter_IPC::toInterpreter::DeviceCommand& msg);
      void processExtraQuery(const interpreter_IPC::toInterpreter::ExtraQuery& msg);
      void processManuallyDeviceCreation(const interpreter_IPC::toInterpreter::ManuallyDeviceCreation& msg);
      void processDeviceRemoved(const interpreter_IPC::toInterpreter::DeviceRemoved& msg);

      void setInitialized();

      static void fillHistorizable(boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> in,
                                   interpreter_IPC::toYadoms::Historizable* out);
      static void fillCapacity(const shared::plugin::yPluginApi::CStandardCapacity& in,
                               interpreter_IPC::toYadoms::Capacity* out);

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
   };
} // namespace interpreter_cpp_api	


