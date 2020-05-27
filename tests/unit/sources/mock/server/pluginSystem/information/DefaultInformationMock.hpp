// Includes needed to compile tested classes
#include "../../../../../../../sources/shared/shared/plugin/information/IInformation.h"

class CDefaultInformationMock : public shared::plugin::information::IInformation
{
public:
   virtual ~CDefaultInformationMock()
   {
   }

   // shared::plugin::IInformation implementation
   const std::string& getType() const override
   {
      static const std::string str("mock");
      return str;
   }

   const shared::versioning::CSemVer& getVersion() const override
   {
      static const shared::versioning::CSemVer version("1.2.3-beta");
      return version;
   }

   const std::string& getAuthor() const override
   {
      static const std::string str("YadomsTeam");
      return str;
   }

   const std::string& getUrl() const override
   {
      static const std::string str("www.yadoms.com");
      return str;
   }

   std::string getIdentity() const override
   {
      std::ostringstream formatedInformations;
      formatedInformations << getType() << " v" << getVersion();
      return formatedInformations.str();
   }

   std::string toString() const override
   {
      std::ostringstream formatedInformations;
      formatedInformations << getIdentity() << " by " << getAuthor() << " (" << getUrl() << ")";
      return formatedInformations.str();
   }

   bool isSupportedOnThisPlatform() const override
   {
      return true;
   }

   bool getSupportManuallyCreatedDevice() const override
   {
      return false;
   } 
   
   bool getSupportDeviceRemovedNotification() const override
   {
      return false;
   }

   boost::shared_ptr<const shared::CDataContainer> getPackage() const override
   {
      return boost::shared_ptr<const shared::CDataContainer>();
   }

   const boost::filesystem::path& getPath() const override
   {
      static boost::filesystem::path emptyPath;
      return emptyPath;
   }

   // [END] shared::plugin::IInformation implementation
};

