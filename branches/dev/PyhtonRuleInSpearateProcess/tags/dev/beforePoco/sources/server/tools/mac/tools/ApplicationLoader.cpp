#include "stdafx.h"
#include "ApplicationLoader.h"

namespace tools {

   int CApplicationLoader::run(const IApplication & application, int argc, char ** argv)
   {
      return application.run(argc, argv);
   }
   
} //namespace tools
