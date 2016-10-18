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
         //--------------------------------------------------------------
         /// \brief	                        Default constructor (only internal usage)
         //--------------------------------------------------------------
         CException()
         {
         }

      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         /// \param[in]  message             Exception message
         //--------------------------------------------------------------
         explicit CException(const char* message)
         {
            if (message != NULL)
               m_message = std::string(message);
         }

         explicit CException(const std::string& message)
            : m_message(message)
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CException() throw()
         {
         }

         //--------------------------------------------------------------
         /// \brief	    Build full message explaining exception reason
         /// \return     message explaining exception reason
         //--------------------------------------------------------------
         virtual char const* what() const throw() override { return m_message.c_str(); }

      protected:
         //--------------------------------------------------------------
         /// \brief      Message container
         //--------------------------------------------------------------
         std::string m_message;
      };
   }
} // namespace shared::exception
