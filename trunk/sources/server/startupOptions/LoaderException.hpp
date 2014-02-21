//
// Yadoms startup options loader by command line and file
//
#pragma once

#include "IStartupOptions.h"
#include <shared/Exceptions/Exception.hpp>

namespace startupOptions
{

   //--------------------------------------------------------------
   /// \class Configuration loading error exception
   //--------------------------------------------------------------
   class CLoaderException : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  optionsDecription   Full options description structure
      /// \param[in]  message             Extended message (useful if error in the command line)
      //--------------------------------------------------------------
      CLoaderException(const boost::program_options::options_description& optionsDecription,
         const char* message = NULL)
         :CException(""), m_optionsDecription(optionsDecription)
      {
         // If no error message was specified, it's not an error (help was invoked)
         m_error = message ? true : false;

         std::ostringstream s;

         if (message)
            s << message << std::endl << std::endl;

         s << "Command line usage :" << std::endl;
         s << "   Yadoms [options]"  << std::endl << std::endl;
         s << m_optionsDecription << std::endl;

         m_message = s.str();
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CLoaderException() throw()
      {
      }

      //--------------------------------------------------------------
      /// \brief	    Get if exception cause is a command line usage error
      /// \return     true is command line arguments are wrong, false is help was invoked
      //--------------------------------------------------------------
      virtual bool isError() const throw() { return m_error; }

   protected:
      bool m_error;
      const boost::program_options::options_description& m_optionsDecription;
   };

} // namespace startupOptions