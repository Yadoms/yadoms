#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <toPlugin.pb.h>


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

   bool stopRequested() const;

   // shared::script::yScriptApi::IYScriptApi implementation
   void setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state, const std::string & customMessageId = std::string()) override;
   bool deviceExists(const std::string& device) const override;
   shared::CDataContainer getDeviceDetails(const std::string& device) const override;
   void declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override;
   bool keywordExists(const std::string& device, const std::string& keyword) const override;
   bool keywordExists(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword) const override;
   void declareKeyword(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override;
   std::string getRecipientValue(int recipientId, const std::string& fieldName) const override;
   std::vector<int> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const override;
   bool recipientFieldExists(const std::string& fieldName) const override;
   void historizeData(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& data) override;
   void historizeData(const std::string& device, const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect) override;
   const shared::plugin::information::IInformation& getInformation() const override;
   shared::CDataContainer getConfiguration() const override;
   shared::event::CEventHandler & getEventHandler() override;
   // [END] shared::script::yScriptApi::IYScriptApi implementation

   void onReceive(const unsigned char* message, size_t messageSize);

protected:
   //--------------------------------------------------------------
   /// \brief	Send a request
   /// \param[in] request Request to send
   //--------------------------------------------------------------
//TODO   void sendRequest(const toYadoms::msg& request) const;

   void processSystem(const toPlugin::System& msg);

private:
   bool m_stopRequested;
};
