#include "stdafx.h"
#include "OperatingSystem.h"

namespace tools {

   const std::string COperatingSystem::m_name = "Windows";

   const std::string & COperatingSystem::getName()
   {
      return m_name;
   }
   
} //namespace tools
