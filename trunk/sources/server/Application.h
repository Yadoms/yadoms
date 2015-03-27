#pragma once
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include "startupOptions/Loader.h"
#include "IRunningInformation.h"
#include "ApplicationStopHandler.h"
#include <shared/event/EventHandler.hpp>

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
   ///\brief Running information
   //-----------------------------
   boost::shared_ptr<IRunningInformation> m_runningInformation;

   //-----------------------------
   ///\brief Stop handler
   //-----------------------------
   boost::shared_ptr<CApplicationStopHandler> m_stopHandler;


   //-----------------------------
   ///\brief Event handler used by supervisor
   //-----------------------------
   boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;

   //-----------------------------
   ///\brief Event code
   //-----------------------------
   enum
   {
      kSupervisorIsStopped = shared::event::kUserFirstId,
      kTerminationRequested
   };
};
