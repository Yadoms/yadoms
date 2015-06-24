#pragma once

namespace communication { namespace command {

   //-----------------------------------
   ///\brief Class which contains a command result
   //-----------------------------------
   class CResult
   {
   public:
      //-----------------------------------
      ///\brief Create a success result
      //-----------------------------------
      static CResult CreateSuccess();

      //-----------------------------------
      ///\brief Create an error message
      ///\param [in] errorMessage the error message
      //-----------------------------------
      static CResult CreateError(const std::string & errorMessage);

   protected:
      //-----------------------------------
      ///\brief Constructor
      ///\param [in] result The result of the command
      //-----------------------------------
      CResult(bool result);

      //-----------------------------------
      ///\brief Constructor
      ///\param [in] result The result of the command
      ///\param [in] errorMessage The error message
      //-----------------------------------
      CResult(bool result, const std::string & errorMessage);

   public:
      //-----------------------------------
      ///\brief Destructor
      //-----------------------------------
      virtual ~CResult();

      //-----------------------------------
      ///\brief Get the command result
      ///\return The command result
      //-----------------------------------
      bool isSuccess() const;

      //-----------------------------------
      ///\brief Get the error message
      ///\return The error message
      //----------------------------------
      const std::string& getErrorMessage() const;

   private:
      //-----------------------------------
      ///\brief The command result
      //-----------------------------------
      bool m_result;

      //-----------------------------------
      ///\brief The error message
      //-----------------------------------
      std::string m_errorMessage;
   };

} //namespace command
} //namespace communication
