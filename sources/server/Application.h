#pragma once
#include <Poco/Util/ServerApplication.h>
#include "startupOptions/StartupOptions.h"
#include "IRunningInformation.h"
#include "IPathProvider.h"
#include "logging/LogConfiguration.h"


//-----------------------------
///\brief Yadoms application
//-----------------------------
class CYadomsServer final : public Poco::Util::ServerApplication
{
public:
   CYadomsServer();
   ~CYadomsServer() override = default;

protected:
   // Poco::Util::ServerApplication implementation
   void initialize(Application& self) override;
   void uninitialize() override;
   void defineOptions(Poco::Util::OptionSet& options) override;
   int main(const ArgVec&) override;
   // [END] Poco::Util::ServerApplication implementation

   void handleHelp(const std::string& name, const std::string& value);
   void displayHelp() const;

   void handleVersion(const std::string& name, const std::string& value);
   static void displayVersion();
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

   //-----------------------------
   ///\brief Log configuration
   //-----------------------------
   boost::shared_ptr<logging::CLogConfiguration> m_logConfiguration;
};
