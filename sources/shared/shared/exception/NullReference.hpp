#pragma once
#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CNullReference : public CException
   {
   public:
      explicit CNullReference(const char* reference = nullptr)
         : CException(makeExceptionDetails(reference))
      {
      }

      ~CNullReference() noexcept override = default;

   private:
      static std::string makeExceptionDetails(const char* reference)
      {
         std::ostringstream s;
         s << "Null reference exception" << std::endl;
         if (reference)
            s << "Exception details : " << reference << std::endl << std::endl;
         return s.str();
      }
   };
}
