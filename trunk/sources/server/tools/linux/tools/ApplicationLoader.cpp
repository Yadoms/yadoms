#include "stdafx.h"
#include "ApplicationLoader.h"

#include "../../tools/IApplication.h"

namespace tools {

   int CApplicationLoader::run(IApplication & application, int argc, char ** argv)
   {
      //configure application with command line parameters
      try
      {
         application.configure(argc, argv);
      }
      catch (startupOptions::CLoaderException& e)
      {
         if (e.isError())
         {
            YADOMS_LOG(fatal) << e.what();
            return 1;
         }

         // Help was invoked, just print to console (independent to log level, so not use log)
         std::cout << e.what();
         return 0;
      }

      return application.run();
   }
   
} //namespace tools
