#pragma once
#include "tools/IApplication.h"
#include "Supervisor.h"
#include "startupOptions/IStartupOptions.h"
#include "IRunningInformation.h"

//-----------------------------
///\brief Yadoms application
//-----------------------------
class CApplication : public tools::IApplication
{
public:
   //-----------------------------
   ///\brief Constructor
   //-----------------------------
   CApplication();
   
   //-----------------------------
   ///\brief Destructor
   //-----------------------------
   virtual ~CApplication();
   
   // tools::IApplication implementation
   virtual void configure(int argc, char ** argv);
   virtual int run(); 
   virtual void stop(boost::function<void()> & callbackAfterStopped);
   // [END] tools::IApplication implementation
private:
   boost::shared_ptr<CSupervisor> m_supervisor;
   boost::shared_ptr<startupOptions::IStartupOptions> m_startupOptions;
   boost::shared_ptr<IRunningInformation> m_runningInformation;
};
