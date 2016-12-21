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
                       const std::string& customMessageId = "",
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
      return "";
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
   
   std::vector<std::string> getAllDevices() const
   {
      return std::vector<std::string>();
   }

   shared::CDataContainer getConfiguration() override
   {
      return m_defaultConfiguration;
   }

   shared::CDataContainer getDeviceConfiguration(const std::string& device) const  override
   {
      return shared::CDataContainer();
   }

   void updateDeviceConfiguration(const std::string& device, const shared::CDataContainer& configuration) const override
   {

   }

   void updateDeviceDetails(const std::string& device, const shared::CDataContainer& details) const override
   {

   }

   std::string getDeviceModel(const std::string& device) const override
   {
      return "model";
   }

   void updateDeviceModel(const std::string& device,      const std::string& model) const override
   {

   }

   void removeDevice(const std::string& device) override
   {

   }


   void declareKeywords(const std::string& device, const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords) override
   {

   }

   void removeKeyword(const std::string& device, const std::string& keyword) override
   {

   }

   std::vector<std::string> getAllKeywords(const std::string& device) const override
   {
      return std::vector<std::string>();
   }

   const boost::filesystem::path& getDataPath() const override
   {
      static const boost::filesystem::path path;
      return path;
   }

   class CMockupYadomsInformation : public shared::plugin::information::IYadomsInformation
   {
   public:
      bool developperMode() const override
      {
         return false;
      }

      shared::versioning::CVersion version() const override
      {
         return shared::versioning::CVersion("1.0.0");
      }

      class Location : public shared::ILocation
      {
      public:
         double latitude() const override
         {
            return 0.0;
         }

         double longitude() const override
         {
            return 0.0;
         }

         virtual double altitude() const override
         {
            return 0.0;
         }
      };

      boost::shared_ptr<const shared::ILocation> location() const override
      {
         static boost::shared_ptr<const shared::ILocation> loc = boost::shared_ptr<shared::ILocation>(new Location());
         return loc;
      }
   };

   //-----------------------------------------------------
   /// \brief	    return information on Yadoms (version, developper mode state, location...)
   /// \return     Yadoms information
   //-----------------------------------------------------
   boost::shared_ptr<const shared::plugin::information::IYadomsInformation> getYadomsInformation() const override
   {
      static boost::shared_ptr<const shared::plugin::information::IYadomsInformation> yi = boost::shared_ptr<shared::plugin::information::IYadomsInformation>(new CMockupYadomsInformation());
      return yi;
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

