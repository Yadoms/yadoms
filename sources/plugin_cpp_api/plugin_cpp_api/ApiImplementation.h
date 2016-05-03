#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <plugin_IPC/plugin_IPC.h>


//-----------------------------------------------------
///\brief The Yadoms plugin API implementation for the CPP plugin API
//-----------------------------------------------------
class CApiImplementation : public shared::plugin::yPluginApi::IYPluginApi
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

   // shared::script::yScriptApi::IYScriptApi implementation
   void setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state,
                       const std::string& customMessageId = std::string()) override;
   bool deviceExists(const std::string& device) const override;
   shared::CDataContainer getDeviceDetails(const std::string& device) const override;
   void declareDevice(const std::string& device,
                      const std::string& model,
                      const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override;
   bool keywordExists(const std::string& device,
                      const std::string& keyword) const override;
   bool keywordExists(const std::string& device,
                      const shared::plugin::yPluginApi::historization::IHistorizable& keyword) const override;
   void declareKeyword(const std::string& device,
                       const shared::plugin::yPluginApi::historization::IHistorizable& keyword,
                       const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override;
   void declareKeyword(const std::string& device,
                       const std::string& capacityName,
                       const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode,
                       const std::string& name,
                       const shared::plugin::yPluginApi::EKeywordDataType& type,
                       const std::string& units,
                       const shared::CDataContainer& typeInfo,
                       const shared::plugin::yPluginApi::historization::EMeasureType& measure,
                       const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override
   {
   }; //TODO à virer

   std::string getRecipientValue(int recipientId,
                                 const std::string& fieldName) const override;
   std::vector<int> findRecipientsFromField(const std::string& fieldName,
                                            const std::string& expectedFieldValue) const override;
   bool recipientFieldExists(const std::string& fieldName) const override;
   void historizeData(const std::string& device,
                      const shared::plugin::yPluginApi::historization::IHistorizable& data) override;
   void historizeData(const std::string& device,
                      const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect) override;
   boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const override;
   shared::CDataContainer getConfiguration() override;
   shared::event::CEventHandler& getEventHandler() override;
   // [END] shared::script::yScriptApi::IYScriptApi implementation

   void onReceive(boost::shared_ptr<const unsigned char[]> message, size_t messageSize);

   void waitInitialized() const;

protected:
   //--------------------------------------------------------------
   /// \brief	Send a request
   /// \param[in] request Request to send
   //--------------------------------------------------------------
   void send(const toYadoms::msg& msg) const;
   void send(const toYadoms::msg& msg,
             boost::function1<bool, const toPlugin::msg&> checkExpectedMessageFunction,
             boost::function1<void, const toPlugin::msg&> onReceiveFunction) const;

   void processSystem(const toPlugin::System& msg);
   void processPluginInformation(const toPlugin::Information& msg);
   void processUpdateConfiguration(const toPlugin::Configuration& msg);
   void processBindingQuery(const toPlugin::BindingQuery& msg);

   void setInitialized();

private:
   mutable std::condition_variable m_initializationCondition;
   mutable std::mutex m_initializationConditionMutex;
   bool m_initialized;

   bool m_stopRequested;

   shared::event::CEventHandler m_pluginEventHandler;


   // The message queue buffer, localy used but defined here to be allocated only once
   boost::shared_ptr<unsigned char[]> m_mqBuffer;
   boost::shared_ptr<boost::interprocess::message_queue> m_sendMessageQueue;

   mutable boost::recursive_mutex m_onReceiveHookMutex;
   mutable boost::function1<bool, const toPlugin::msg&> m_onReceiveHook;

   boost::shared_ptr<shared::plugin::information::IInformation> m_pluginInformation;
};
