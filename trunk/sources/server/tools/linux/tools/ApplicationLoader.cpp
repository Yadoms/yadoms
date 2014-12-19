#include "stdafx.h"
#include "ApplicationLoader.h"

#include "../../tools/IApplication.h"

namespace tools {

   int CApplicationLoader::run(IApplication & application, int argc, char ** argv)
   {
      //configure application with command line parameters
      application.configure(argc, argv);

      return application.run();
   }
   
} //namespace tools
