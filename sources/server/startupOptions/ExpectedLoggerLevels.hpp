//
// Custom validator for logger levels
//
#pragma once
#include <boost/assign.hpp>

namespace startupOptions
{

   //--------------------------------------------------------------
   /// \class logger level option
   /// This class check if the logger level given as option exists
   //--------------------------------------------------------------
   class CExpectedLoggerLevels
   {
   public:
      //--------------------------------------------------------------
      /// \brief	         Constructor
      /// \param[in] path  The logger level
      //--------------------------------------------------------------
      CExpectedLoggerLevels(const std::string& level)
         :m_level(level)  {}

      //--------------------------------------------------------------
      /// \brief	    Default constructor
      //--------------------------------------------------------------
      CExpectedLoggerLevels() {}

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CExpectedLoggerLevels() {}

      //--------------------------------------------------------------
      /// \brief	    Affectation operator
      /// \note       Needed for implementation of po::value::default_value()
      //--------------------------------------------------------------
      CExpectedLoggerLevels& operator=(const CExpectedLoggerLevels& src)
      { 
         m_level = src.m_level; 
         return *this;
      }

   public:
      //--------------------------------------------------------------
      /// \brief	    Check the provided logger level
      /// \param[in] level  The logger level
      /// \return     true if level exists
      //--------------------------------------------------------------
      static bool validate(const std::string& level)
      {
         static const std::set<std::string> acceptedLevels = boost::assign::list_of("none")("fatal")("critical")("error")("warning")("notice")("information")("debug")("trace");
         if (acceptedLevels.find(level) == acceptedLevels.end())
            return false;
         return true;
      }

      //--------------------------------------------------------------
      /// \brief	    Logger level getter
      /// \return     Logger leve
      //--------------------------------------------------------------
      const std::string& get() const
      { return m_level; }

   protected:
      std::string m_level;
   };

} // namespace startupOptions