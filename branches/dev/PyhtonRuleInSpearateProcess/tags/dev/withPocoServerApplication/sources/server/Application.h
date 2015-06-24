#pragma once
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/OptionSet.h>
#include "startupOptions/Loader.h"
#include "IApplicationStopHandler.h"

//-----------------------------
///\brief Yadoms application
//-----------------------------
class CYadomsServer : public Poco::Util::ServerApplication, IApplicationStopHandler
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

   // IApplicationStopHandler implementation
   virtual void requestToStop(EStopMode stopMode) const;
   // [END] IApplicationStopHandler implementation

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
   startupOptions::CStartupOptions m_startupOptions;
};
