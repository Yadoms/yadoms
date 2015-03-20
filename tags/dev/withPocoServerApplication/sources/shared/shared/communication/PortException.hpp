#pragma once
#include <shared/exception/Exception.hpp>

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	Exception handling for communication port
   //--------------------------------------------------------------
   class CPortException : public exception::CException
   {
   public:
      enum ECode
      {
         kConnectionClosed,
         kConnectionError
      };

   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  code                Connection error code
      /// \param[in]  message             Exception message
      //--------------------------------------------------------------
      CPortException(ECode code, const std::string& message)
         :CException(codeLabelsToString(code) + ", " + message.c_str())
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CPortException() throw()
      {
      }

   private:
      //--------------------------------------------------------------
      /// \brief                 Get the code labels
      /// \param[in] code        The error code
      /// \return                The error label
      //--------------------------------------------------------------
      static const std::string codeLabelsToString(ECode code)
      {
         static const std::map<ECode, std::string> CodeLabels = boost::assign::map_list_of
            (kConnectionClosed, "connection was closed")
            (kConnectionError, "connection error");

         std::map<ECode, std::string>::const_iterator itCodeLabel = CodeLabels.find(code);
         if (itCodeLabel == CodeLabels.end())
            return boost::lexical_cast<std::string>(code);

         return itCodeLabel->second;
      }
   };

} } // namespace shared::communication
