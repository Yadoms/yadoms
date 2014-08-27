#include "stdafx.h"
#include "ApplicationLoader.h"

#include "../../tools/IApplication.h"

namespace tools {

   int CApplicationLoader::run(IApplication & application, int argc, char ** argv)
   {
      return application.run(argc, argv);
   }
   
} //namespace tools
