#include "stdafx.h"
#include "OperatingSystem.h"

namespace tools {

   const std::string COperatingSystem::m_name = "Mac OSX";

   const std::string & COperatingSystem::getName()
   {
      return m_name;
   }
   
} //namespace tools
