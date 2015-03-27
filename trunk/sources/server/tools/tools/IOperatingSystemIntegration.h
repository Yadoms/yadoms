#pragma once

namespace tools
{
   //-----------------------------
   ///\brief Interface for operating system integration
   //-----------------------------
   class IOperatingSystemIntegration
   {
   public:
      //-----------------------------
      ///\brief Virtual destructor
      //-----------------------------   
      virtual ~IOperatingSystemIntegration() {}
      
      //-----------------------------
      ///\brief Integrate Yadoms in operating system
      //-----------------------------   
      virtual void configure() = 0;
   };

} // namespace tools
