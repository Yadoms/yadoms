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
      CNullReference(const char * reference = NULL)
      {
         std::ostringstream s;
         s << "Null reference exception" << std::endl;
         if (reference)
            s << "Exception details : " << reference << std::endl << std::endl;
         m_message = s.str();
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CNullReference() throw()
      {
      }
   };

} } // namespace shared::exception
