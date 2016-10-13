//
// Yadoms startup options loader by command line and file
//
#pragma once


#include <Poco/Util/Validator.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionException.h>

namespace startupOptions
{
   //--------------------------------------------------------------
   /// \class Validator for list of values not handle by enumerations
   //--------------------------------------------------------------
   class CListValidator : public Poco::Util::Validator
   {
   public:
      //--------------------------------------------------------------
      /// \brief Constructor
      /// \param [in] availableValues The list of all available values
      //--------------------------------------------------------------
      explicit CListValidator(std::vector<std::string> & availableValues)
      :m_availableValues(availableValues)
      {
      }
         
      //--------------------------------------------------------------
      /// \brief Destructor
      //--------------------------------------------------------------
      virtual ~CListValidator()
      {
      }
      
      //Poco::Util::Validator implementation
      void validate(const Poco::Util::Option& option, const std::string& value)
      {
         if (std::find(m_availableValues.begin(), m_availableValues.end(), value) == m_availableValues.end())
            throw Poco::Util::OptionException();
      }
      //[END] - Poco::Util::Validator implementation
      
    private:
      //--------------------------------------------------------------
      /// \brief Reference to all available values
      //--------------------------------------------------------------
      std::vector<std::string> & m_availableValues;
   };

} // namespace startupOptions