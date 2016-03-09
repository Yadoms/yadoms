

// Includes needed to compile tested classes
#include "../../../../../../../sources/shared/shared/plugin/information/IInformation.h"

class CDefaultInformationMock : public shared::plugin::information::IInformation
{
public:
   virtual ~CDefaultInformationMock() {}
   // shared::plugin::IInformation implementation
   virtual const std::string& getType() const { static const std::string str("mock"); return str; }
   virtual const std::string& getVersion() const { static const std::string str("99"); return str; }
   virtual const shared::versioning::EReleaseType getReleaseType() const { return shared::versioning::EReleaseType::kBeta; }
   virtual const std::string& getAuthor() const { static const std::string str("YadomsTeam"); return str; }
   virtual const std::string& getUrl() const { static const std::string str("www.yadoms.com"); return str; }
   virtual std::string getIdentity() const { std::ostringstream formatedInformations; formatedInformations << getType() << " v" << getVersion() << "[" << boost::lexical_cast<std::string>(getReleaseType()) << "]"; return formatedInformations.str(); }
   virtual std::string toString() const { std::ostringstream formatedInformations; formatedInformations << getIdentity() << " by " << getAuthor() << " (" << getUrl() << ")"; return formatedInformations.str(); }
   virtual bool isSupportedOnThisPlatform() const { return true; }
   virtual bool getSupportManuallyCreatedDevice() const { return false; }
   virtual shared::CDataContainer getPackage() const { return shared::CDataContainer(); }
   // [END] shared::plugin::IInformation implementation
};