#pragma once

#include "../../tools/IApplication.h"

namespace tools {

   class CApplicationLoader
   {
   public:
      static int run(const IApplication & application, int argc, char ** argv);
      
   };

} //namespace tools
