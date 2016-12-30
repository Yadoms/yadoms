#pragma once

#include <shared/exception/Exception.hpp>

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Exception handling for process
      //--------------------------------------------------------------
      class [[deprecated("Use std::runtime_error instead")]] CProcessException : public shared::exception::CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         /// \param[in]  message             Exception message
         //--------------------------------------------------------------
         explicit CProcessException(const std::string& message)
            :CException(message)
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CProcessException() throw()
         {
         }
      };
   }
} // namespace shared::process


