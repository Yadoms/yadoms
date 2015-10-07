

// Includes needed to compile tested classes
#include "../../../../../sources/shared/shared/plugin/yPluginApi/IYPluginApi.h"

// Moke IYPluginApi
namespace yApi = shared::plugin::yPluginApi;
class CDefaultYPluginApiMock : public yApi::IYPluginApi
{
public:
   struct Device { std::string m_model; shared::CDataContainer m_details; };
   struct Keyword { std::string m_device; std::string m_keyword; std::string m_capacity; shared::CDataContainer m_details; };
   struct Data { std::string m_device; std::string m_keyword; std::string m_capacity; std::string m_value; };
public:
   virtual ~CDefaultYPluginApiMock() {}
   // IYPluginApi implementation
   virtual shared::event::CEventHandler & getEventHandler() { return m_defaultEventHandler; }
   virtual bool deviceExists(const std::string& device) const { return m_devices.find(device) != m_devices.end(); }
   virtual const shared::CDataContainer getDeviceDetails(const std::string& device) const { return m_devices.find(device)->second.m_details; }
   virtual void declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   {
      Device dev = { model, details }; m_devices[device] = dev;
   }
   virtual bool keywordExists(const std::string& device, const std::string& keyword) const { return m_keywords.find(keyword) != m_keywords.end(); }
   virtual bool keywordExists(const std::string& device, const yApi::historization::IHistorizable& keyword) const { return false; }
   virtual void declareKeyword(const std::string& device, const yApi::historization::IHistorizable& keyword, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer)
   {
      Keyword kw = { device, keyword.getKeyword(), keyword.getCapacity(), details }; m_keywords[keyword.getKeyword()] = kw;
   }
   virtual std::string getRecipientValue(int recipientId, const std::string& fieldName) const { return shared::CStringExtension::EmptyString; }
   virtual std::vector<int> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const { return m_recipients; }
   virtual bool recipientFieldExists(const std::string& fieldName) const { return true; }
   virtual void historizeData(const std::string& device, const yApi::historization::IHistorizable& keyword)
   {
      Data data = { device, keyword.getKeyword(), keyword.getCapacity(), keyword.formatValue() }; m_data.push_back(data);
   }
   virtual void historizeData(const std::string& device, const std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > & dataVect)
   {
      for (std::vector<boost::shared_ptr<yApi::historization::IHistorizable> >::const_iterator keyword = dataVect.begin(); keyword != dataVect.end(); ++keyword)
      {
         Data data = { device, (*keyword)->getKeyword(), (*keyword)->getCapacity(), (*keyword)->formatValue() };
         m_data.push_back(data);
      }
   }
   virtual const shared::plugin::information::IInformation& getInformation() const { return m_defaultInformation; }
   virtual const boost::filesystem::path getPluginPath() const { return boost::filesystem::initial_path<boost::filesystem::path>(); }
   virtual shared::CDataContainer getConfiguration() const { return m_defaultConfiguration; }
   virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message) {}
   // [END] IYPluginApi implementation

   const std::map<std::string, Device>& getDevices() const { return m_devices; }
   const std::map<std::string, Keyword>& getKeywords() const { return m_keywords; }
   const std::vector<Data>& getData() const { return m_data; }
protected:
   shared::event::CEventHandler m_defaultEventHandler;
   CDefaultInformationMock m_defaultInformation;
   shared::CDataContainer m_defaultConfiguration;
   std::map<std::string, Device> m_devices;
   std::map<std::string, Keyword> m_keywords;
   std::vector<Data> m_data;
   std::vector<int> m_recipients;
};