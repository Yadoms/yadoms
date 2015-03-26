#pragma once
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include "startupOptions/Loader.h"
#include "IApplicationStopHandler.h"
#include "IRunningInformation.h"

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
   void initialize(Poco::Util::Application& self);
   void uninitialize();
   void defineOptions(Poco::Util::OptionSet& options);
   void handleHelp(const std::string& name, const std::string& value);
   void displayHelp();
   int main(const ArgVec& args);

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
   ///\brief Stop handler
   //-----------------------------
   boost::shared_ptr<IApplicationStopHandler> m_stopHandler; 
   
   //-----------------------------
   ///\brief Stop handler
   //-----------------------------
   boost::shared_ptr<IRunningInformation> m_runningInformation;
};
