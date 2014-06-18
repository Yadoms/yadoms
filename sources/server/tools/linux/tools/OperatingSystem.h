#pragma once

namespace tools {

   class COperatingSystem
   {
   public:
      static const std::string & getName();
      
   private:
      static const std::string m_name;
   };

} //namespace tools