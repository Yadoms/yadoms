#include "stdafx.h"
#include "ApplicationLoader.h"

namespace tools {

   int CApplicationLoader::run(IApplication & application, int argc, char ** argv)
   {
      return application.run();
   }
   
} //namespace tools
