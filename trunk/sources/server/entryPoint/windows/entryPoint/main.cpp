#include "stdafx.h"
#include "Application.h"
#include "tools/ApplicationLoader.h"
#include "YadomsServiceInformation.h"


/*
\brief Main application entry point
*/
int main(int argc, char** argv)
{
   try
   {
      //create the app
      CApplication app;
         
      //create the yadoms service information (windows only)
      entryPoint::CYadomsServiceInformation ysi;
         
      //run the application (as service with install/remove feature , or as console app)
      return tools::CApplicationLoader::run(app, ysi, argc, argv);
      }
   catch(...)
   {
      //dual logging in case logger fails/throws
      std::cout << "An unhandled exception occurred. Yadoms is now stopped" << std::endl;
   }


   return 0;
}

