//
// Options.h
//
// Yadoms-configuration management singleton
//
#pragma once

#include <boost/log/trivial.hpp>


class COptions
{
private:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \note       Declared as private to implement singleton
   //--------------------------------------------------------------
   COptions();

public:
   //--------------------------------------------------------------
   /// \brief	    Get the log level
   /// \return     Configured log level
   //--------------------------------------------------------------
   static boost::log::trivial::severity_level getLogLevel();


private:
   //--------------------------------------------------------------
   /// \brief	    The singleton instance
   /// \note       We don't want COptions to be create at application
   ///             loading, to be sure to get the command line parameters.
   //--------------------------------------------------------------
   static COptions* m_instance;

   //--------------------------------------------------------------
   /// \brief	    Get (create if necessary) the singleton instance
   /// \return     The COptions singleton
   //--------------------------------------------------------------
   static COptions* instance();
};


