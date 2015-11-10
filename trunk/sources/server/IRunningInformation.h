#pragma once

#include <shared/versioning/VersionInformation.h>

//-----------------------------------------------------------------------------
/// \class   Interface on server system informations
//-----------------------------------------------------------------------------
class IRunningInformation
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~IRunningInformation() {}

   //-----------------------------------------------------------------------------
   /// \brief		                     get the startup date/time
   /// \return  	                     the startup date/time
   //-----------------------------------------------------------------------------
   virtual const boost::posix_time::ptime  & getStartupDateTime() const = 0;
   
   //-----------------------------------------------------------------------------
   /// \brief		                     get the software version
   /// \return  	                     the software version
   //-----------------------------------------------------------------------------
   virtual const shared::versioning::CVersionInformation  & getSoftwareVersion() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the operating system name (windows, linux,....)
   /// \return  	                     the operating system name
   //-----------------------------------------------------------------------------
   virtual const std::string getOperatingSystemName() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the full executable path
   /// \return  	                     the full executable path (path + filename)
   //-----------------------------------------------------------------------------
   virtual const std::string & getExecutablePath() const = 0;
};

