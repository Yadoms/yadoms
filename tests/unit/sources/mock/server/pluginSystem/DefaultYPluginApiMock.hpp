// Includes needed to compile tested classes
#include "../../../../../../sources/shared/shared/plugin/yPluginApi/IYPluginApi.h"
#include "information/DefaultInformationMock.hpp"

// Moke IYPluginApi
namespace yApi = shared::plugin::yPluginApi;

class CDefaultYPluginApiMock : public yApi::IYPluginApi
{
public:
   struct Device
   {
      std::string m_model;
      shared::CDataContainer m_details;
   };

   struct Keyword
   {
      std::string m_device;
      std::string m_keyword;
      std::string m_capacity;
      shared::CDataContainer m_details;
   };

   struct Data
   {
      std::string m_device;
      std::string m_keyword;
      std::string m_capacity;
      std::string m_value;
   };

   CDefaultYPluginApiMock()
      : m_defaultInformation(boost::make_shared<CDefaultInformationMock>())
   {
   }

   virtual ~CDefaultYPluginApiMock()
   {
   }

   // IYPluginApi implementation
   shared::event::CEventHandler& getEventHandler() override
   {
      return m_defaultEventHandler;
   }

   void setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state,
                       const std::string& customMessageId = std::string(),
                       const std::map<std::string, std::string>& customMessageData = std::map<std::string, std::string>()) override
   {
   }

   void declareDevice(const std::string& device,
                      const std::string& model,
                      boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                      const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override
   {
      Device dev = {model, details};
      m_devices[device] = dev;
   }

   void declareDevice(const std::string& device,
                      const std::string& model,
                      const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords = std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>(),
                      const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override
   {
      Device dev = {model, details};
      m_devices[device] = dev;
      std::for_each(keywords.begin(),
                    keywords.end(),
                    [&](boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword)
                    {
                       Keyword kw = {device, keyword->getKeyword(), keyword->getCapacity(), details};
                       m_keywords[keyword->getKeyword()] = kw;
                    });
   }

   std::vector<std::string> getAllDevices() const override
   {
      std::vector<std::string> devices;
      for (auto device : m_devices)
         devices.push_back(device.first);
      return devices;
   }

   bool deviceExists(const std::string& device) const override
   {
      return m_devices.find(device) != m_devices.end();
   }

   shared::CDataContainer getDeviceConfiguration(const std::string& device) const override
   {
      return shared::CDataContainer();
   }

   void updateDeviceConfiguration(const std::string& device,
                                  const shared::CDataContainer& configuration) const override
   {
   }

   shared::CDataContainer getDeviceDetails(const std::string& device) const override
   {
      return m_devices.find(device)->second.m_details;
   }

   void updateDeviceDetails(const std::string& device,
                            const shared::CDataContainer& details) const override
   {
      m_devices.find(device)->second.m_details.initializeWith(details);
   }

   std::string getDeviceModel(const std::string& device) const override
   {
      return std::string();
   }

   void updateDeviceModel(const std::string& device,
                          const std::string& model) const override
   {
   }

   void removeDevice(const std::string& device) override
   {
      m_devices.erase(device);
   }

   bool keywordExists(const std::string& device,
                      const std::string& keyword) const override
   {
      return m_keywords.find(keyword) != m_keywords.end();
   }

   bool keywordExists(const std::string& device,
                      boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword) const override
   {
      return false;
   }

   std::vector<std::string> getAllKeywords(const std::string& device) const override
   {
      std::vector<std::string> keywords;
      for (auto keyword:m_keywords)
         keywords.push_back(keyword.first);
      return keywords;
   }

   void removeKeyword(const std::string& device,
                      const std::string& keyword) override
   {
      m_keywords.erase(keyword);
   }

   void declareKeyword(const std::string& device,
                       boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                       const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override
   {
      Keyword kw = {device, keyword->getKeyword(), keyword->getCapacity(), details};
      m_keywords[keyword->getKeyword()] = kw;
   }

   void declareKeywords(const std::string& device,
                        const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords) override
   {
      for (auto keyword:keywords)
         declareKeyword(device, keyword);
   }

   std::string getRecipientValue(int recipientId,
                                 const std::string& fieldName) const override
   {
      return std::string();
   }

   std::vector<int> findRecipientsFromField(const std::string& fieldName,
                                            const std::string& expectedFieldValue) const override
   {
      return m_recipients;
   }

   bool recipientFieldExists(const std::string& fieldName) const override
   {
      return true;
   }

   void historizeData(const std::string& device,
                      boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> data) override
   {
      Data kwData = {device, data->getKeyword(), data->getCapacity(), data->formatValue()};
      m_data.push_back(kwData);
   }

   void historizeData(const std::string& device,
                      const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect) override
   {
      std::for_each(dataVect.begin(),
                    dataVect.end(),
                    [&](boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword)
                    {
                       Data kwData = {device, keyword->getKeyword(), keyword->getCapacity(), keyword->formatValue()};
                       m_data.push_back(kwData);
                    });
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const override
   {
      return m_defaultInformation;
   }

   shared::CDataContainer getConfiguration() override
   {
      return m_defaultConfiguration;
   }

   const boost::filesystem::path& getDataPath() const override
   {
      static const boost::filesystem::path path;
      return path;
   }

   boost::shared_ptr<const shared::plugin::information::IYadomsInformation> getYadomsInformation() const override
   {
      return boost::shared_ptr<const shared::plugin::information::IYadomsInformation>();
   }

   // [END] IYPluginApi implementation

   const std::map<std::string, Device>& getDevices() const
   {
      return m_devices;
   }

   const std::map<std::string, Keyword>& getKeywords() const
   {
      return m_keywords;
   }

   const std::vector<Data>& getData() const
   {
      return m_data;
   }

protected:
   shared::event::CEventHandler m_defaultEventHandler;
   boost::shared_ptr<const shared::plugin::information::IInformation> m_defaultInformation;
   shared::CDataContainer m_defaultConfiguration;
   mutable std::map<std::string, Device> m_devices;
   std::map<std::string, Keyword> m_keywords;
   std::vector<Data> m_data;
   std::vector<int> m_recipients;
};

