#pragma once
#include "tools/IApplication.h"
#include "Supervisor.h"

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
   virtual int run(int argc, char ** argv); 
   virtual void stop(boost::function<void()> & callbackAfterStopped);
   // [END] tools::IApplication implementation
private:
   boost::shared_ptr<CSupervisor> m_supervisor;
};
