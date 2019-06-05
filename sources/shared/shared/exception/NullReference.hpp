#pragma once
#include "Exception.hpp"

namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \class Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CNullReference : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CNullReference(const char * reference = NULL)
         :CException(makeExceptionDetails(reference))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CNullReference() noexcept
      {
      }

   private:
      static std::string makeExceptionDetails(const char * reference)
      {
         std::ostringstream s;
         s << "Null reference exception" << std::endl;
         if (reference)
            s << "Exception details : " << reference << std::endl << std::endl;
         return s.str();
      }
   };

} } // namespace shared::exception
