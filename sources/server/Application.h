#pragma once
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include "startupOptions/StartupOptions.h"
#include "IRunningInformation.h"
#include "IPathProvider.h"


//-----------------------------
///\brief Yadoms application
//-----------------------------
class CYadomsServer : public Poco::Util::ServerApplication
{
public:
   //-----------------------------
   ///\brief Constructor
   //-----------------------------
   CYadomsServer();

   //-----------------------------
   ///\brief Destructor
   //-----------------------------
   virtual ~CYadomsServer();

protected:
   void initialize(Poco::Util::Application& self) override;
   void uninitialize() override;
   void defineOptions(Poco::Util::OptionSet& options) override;
   void handleHelp(const std::string& name, const std::string& value);
   void displayHelp() const;
   int main(const ArgVec& args) override;

private:
   //-----------------------------
   ///\brief Indicate if help display is requested
   //-----------------------------
   bool m_helpRequested;

   //-----------------------------
   ///\brief Startup options
   //-----------------------------
   boost::shared_ptr<startupOptions::CStartupOptions> m_startupOptions;
   
   //-----------------------------
   ///\brief Path provider
   //-----------------------------
   boost::shared_ptr<const IPathProvider> m_pathProvider;

   //-----------------------------
   ///\brief Running information
   //-----------------------------
   boost::shared_ptr<IRunningInformation> m_runningInformation;
};
