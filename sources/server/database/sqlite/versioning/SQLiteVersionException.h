#pragma once
#include <shared/exception/Exception.hpp>


   namespace database { 
      namespace sqlite { 
         namespace versioning { 

            //--------------------------------------------------------------
            /// \class Standard exception with some text
            //--------------------------------------------------------------
            class CSQLiteVersionException : public shared::exception::CException
            {
            public:
               //--------------------------------------------------------------
               /// \brief	                        Constructor
               /// \param[in]  message             Exception message
               //--------------------------------------------------------------
               explicit CSQLiteVersionException(const char * message)
                  :CException(message)
               {
               }

               //--------------------------------------------------------------
               /// \brief      Destructor
               //--------------------------------------------------------------
               virtual ~CSQLiteVersionException() throw()
               {
               }
            };

         } //namespace versioning
      } //namespace sqlite
   } //namespace database 

