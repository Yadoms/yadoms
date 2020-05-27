#pragma once
#include <Poco/Util/ServerApplication.h>
#include "startupOptions/StartupOptions.h"
#include "IRunningInformation.h"
#include "IPathProvider.h"


//-----------------------------
///\brief Yadoms application
//-----------------------------
class CYadomsServer : public Poco::Util::ServerApplication
{
public:
   CYadomsServer();
   virtual ~CYadomsServer() = default;

protected:
   // Poco::Util::ServerApplication implementation
   void initialize(Poco::Util::Application& self) override;
   void uninitialize() override;
   void defineOptions(Poco::Util::OptionSet& options) override;
   int main(const ArgVec&) override;
   // [END] Poco::Util::ServerApplication implementation

   void handleHelp(const std::string& name, const std::string& value);
   void displayHelp() const;

   void handleVersion(const std::string& name, const std::string& value);
   void displayVersion() const;
private:
   //-----------------------------
   ///\brief Setup global proxy settings
   //-----------------------------
   void setupProxy() const;
   
   //-----------------------------
   ///\brief Indicate if help display is requested
   //-----------------------------
   bool m_helpRequested;

   //-----------------------------
   ///\brief Startup options
   //-----------------------------
   boost::shared_ptr<const startupOptions::CStartupOptions> m_startupOptions;
   
   //-----------------------------
   ///\brief Path provider
   //-----------------------------
   boost::shared_ptr<const IPathProvider> m_pathProvider;

   //-----------------------------
   ///\brief Running information
   //-----------------------------
   boost::shared_ptr<IRunningInformation> m_runningInformation;
};
