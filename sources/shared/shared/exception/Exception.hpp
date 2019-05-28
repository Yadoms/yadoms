#pragma once

#include <string>
#include <exception>

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Standard exception with some text
      //--------------------------------------------------------------
      class CException : public std::exception
      {
      protected:
         CException() = default;

      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         /// \param[in]  message             Exception message
         //--------------------------------------------------------------
         explicit CException(const char* message)
            :m_message(message)
         {
         }

         explicit CException(const std::string& message)
            : m_message(message)
         {
         }

         virtual ~CException() noexcept = default;

         //--------------------------------------------------------------
         /// \brief	    Build full message explaining exception reason
         /// \return     message explaining exception reason
         //--------------------------------------------------------------
         const char *what() const noexcept override {
            return m_message.what();
         }
      protected:
         std::runtime_error m_message;
      };

      static_assert(std::is_nothrow_copy_constructible<CException>::value, "shared::exception::CException must be nothrow copy constructible");

   }
} // namespace shared::exception
