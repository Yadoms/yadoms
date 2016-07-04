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
                       const std::string& customMessageId = shared::CStringExtension::EmptyString,
                       const std::map<std::string, std::string> & customMessageData = std::map<std::string, std::string>()) override
   {
   }

   bool deviceExists(const std::string& device) const override
   {
      return m_devices.find(device) != m_devices.end();
   }

   shared::CDataContainer getDeviceDetails(const std::string& device) const override
   {
      return m_devices.find(device)->second.m_details;
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
                      const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >& keywords = std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >(),
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

   void declareKeyword(const std::string& device,
                       boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                       const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) override
   {
      Keyword kw = {device, keyword->getKeyword(), keyword->getCapacity(), details};
      m_keywords[keyword->getKeyword()] = kw;
   }

   std::string getRecipientValue(int recipientId,
                                 const std::string& fieldName) const override
   {
      return shared::CStringExtension::EmptyString;
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
                      const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> >& dataVect) override
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
   std::map<std::string, Device> m_devices;
   std::map<std::string, Keyword> m_keywords;
   std::vector<Data> m_data;
   std::vector<int> m_recipients;
};

