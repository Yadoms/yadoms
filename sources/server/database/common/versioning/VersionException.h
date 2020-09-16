#pragma once
#include <shared/exception/Exception.hpp>


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //--------------------------------------------------------------
         /// \class CVersionException Version exception with some text
         //--------------------------------------------------------------
         class CVersionException : public shared::exception::CException
         {
         public:
            //--------------------------------------------------------------
            /// \brief	                        Constructor
            /// \param[in]  message             Exception message
            //--------------------------------------------------------------
            explicit CVersionException(const char* message)
               : CException(message)
            {
            }

            explicit CVersionException(const std::string& message)
               : CException(message.c_str())
            {
            }

            virtual ~CVersionException() throw() = default;
         };
      } //namespace versioning
   } //namespace common
} //namespace database 
