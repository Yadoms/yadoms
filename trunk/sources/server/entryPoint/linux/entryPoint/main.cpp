#include "stdafx.h"
#include "Application.h"
#include "tools/ApplicationLoader.h"

/*
\brief Main application entry point
*/
int main(int argc, char** argv)
{
   try
   {
      CApplication app;
      return tools::CApplicationLoader::run(app, argc, argv);
    }
   catch(...)
   {
      //dual logging in case logger fails/throws
      std::cout << "An unhandled exception occurred. Yadoms is now stopped" << std::endl;
   }


   return 0;
}

