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
         {
            if (message != nullptr)
               m_message = std::string(message);
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
         char const* what() const noexcept override { return m_message.c_str(); }

      protected:
         std::string m_message;
      };
   }
} // namespace shared::exception
