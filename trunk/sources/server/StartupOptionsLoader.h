//
// Yadoms startup options loader by command line and file
//
#pragma once

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>

#include "StartupOptions.h"

//--------------------------------------------------------------
/// \class Configuration loading error exception
//--------------------------------------------------------------
class CStartupOptionsLoaderError : public std::exception
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  optionsDecription   Full options description structure
   /// \param[in]  message             Extended message (usefull if error in the command line)
   //--------------------------------------------------------------
   CStartupOptionsLoaderError(const boost::program_options::options_description& optionsDecription,
      const char* message = NULL);

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CStartupOptionsLoaderError() throw();

   //--------------------------------------------------------------
   /// \brief	    Build full message explaining error reason and command line usage
   /// \return     message explaining error reason and command line usage
   //--------------------------------------------------------------
   virtual const char* what() const throw() { return m_message.c_str(); }

   //--------------------------------------------------------------
   /// \brief	    Get if exception cause is a command line usage error
   /// \return     true is command line arguments are wrong, false is help was invoked
   //--------------------------------------------------------------
   virtual bool isError() const throw() { return m_error; }

protected:
   bool m_error;
   std::string m_message;
   const boost::program_options::options_description& m_optionsDecription;
};


//--------------------------------------------------------------
/// \class Special path option
/// This class check if the path given as option exists
//--------------------------------------------------------------
class CMustExistPathOption
{
public:
   //--------------------------------------------------------------
   /// \brief	         Constructor
   /// \param[in] path  The path
   //--------------------------------------------------------------
   CMustExistPathOption(const std::string& path)
      :m_path(path)  {}

   //--------------------------------------------------------------
   /// \brief	    Default constructor
   //--------------------------------------------------------------
   CMustExistPathOption() {}

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMustExistPathOption() {}

   //--------------------------------------------------------------
   /// \brief	    Affectation operator
   /// \note       Needed for implementation of po::value::default_value()
   //--------------------------------------------------------------
   void operator=(const CMustExistPathOption& src)
   { m_path = src.m_path; }

public:
   //--------------------------------------------------------------
   /// \brief	    Path getter
   /// \return     Path
   //--------------------------------------------------------------
   const std::string& get() const
      { return m_path; }

protected:
   std::string m_path;
};


//--------------------------------------------------------------
/// \class Default application options loader
/// This loader get each option from :
/// - the command line first, if not provided
/// - in config file, if not provided
/// - the default value
//--------------------------------------------------------------
class CStartupOptionsLoader : public IStartupOptions
{
public:
   //--------------------------------------------------------------
   /// \brief	            Constructor
   /// \param[in]  argc    Main argc parameter (from command line)
   /// \param[in]  argv    Main argv parameter (from command line)
   /// \throw              CStartupOptionsAbortExecution
   //--------------------------------------------------------------
   CStartupOptionsLoader(int argc, char** argv);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CStartupOptionsLoader();

   //--------------------------------------------------------------
   // IStartupOptions implementation (see IStartupOptions declaration for details)
   virtual boost::log::trivial::severity_level getLogLevel() const
      { return m_logLevel; }
   virtual unsigned int getPortNumber() const
      { return m_portNumber; }
   virtual const std::string& getDatabaseFile() const
      { return m_databaseFile; }
   virtual const std::string& getHarwarePluginsPath() const
      { return m_hardwarePluginsPath.get(); }
   virtual const std::string& getDevicePluginsPath() const
      { return m_devicePluginsPath.get(); }
   virtual bool getStartXplHubFlag() const
      { return m_startXplHub; }
   // [END] IStartupOptions implementation
   //--------------------------------------------------------------


private:
   //--------------------------------------------------------------
   /// \brief	            Build the supported options list
   //--------------------------------------------------------------
   void buildOptionsDescription();

private:
   static const std::string OptionalConfigFile;

   // Options description
   boost::program_options::options_description m_optionsDescription;

   // Options data
   boost::log::trivial::severity_level m_logLevel;
   unsigned int m_portNumber;
   std::string m_databaseFile;
   CMustExistPathOption m_hardwarePluginsPath;
   CMustExistPathOption m_devicePluginsPath;
   bool m_startXplHub;
};


