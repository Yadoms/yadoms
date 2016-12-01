#pragma once
#include <shared/exception/Exception.hpp>

namespace database
{
   //--------------------------------------------------------------
   /// \class Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CDatabaseException : public shared::exception::CException
   {
   public:
      enum EDatabaseReturnCodes
      {
         kOk = 0,
         kError,
         kConstraintViolation
      };

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CDatabaseException(const std::string& errMsg)
         : CException(std::string(errMsg)),
           m_returnCode(kOk)
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDatabaseException(const std::string& errMsg,
                         const std::string& innerError)
         : CException(std::string(errMsg) + " " + std::string(innerError)),
           m_returnCode(kOk)
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDatabaseException(const std::string& errMsg,
                         EDatabaseReturnCodes returnCode)
         : CException(std::string(errMsg) + " " + std::to_string(returnCode)),
           m_returnCode(returnCode)
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CDatabaseException() throw()
      {
      }

      EDatabaseReturnCodes returnCode() const
      {
         return m_returnCode;
      }

   private:
      EDatabaseReturnCodes m_returnCode;
   };
} //namespace database 


