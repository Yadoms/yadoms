#pragma once

#include <shared/versioning/VersionInformation.h>

//-----------------------------------------------------------------------------
/// \brief   Interface on server system informations
//-----------------------------------------------------------------------------
class IRunningInformation
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~IRunningInformation()
   {
   }

   //-----------------------------------------------------------------------------
   /// \brief		                     get the startup date/time
   /// \return  	                     the startup date/time
   //-----------------------------------------------------------------------------
   virtual const boost::posix_time::ptime& getStartupDateTime() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the software version
   /// \return  	                     the software version
   //-----------------------------------------------------------------------------
   virtual const shared::versioning::CVersionInformation& getSoftwareVersion() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the operating system name (windows, linux,....)
   /// \return  	                     the operating system name
   //-----------------------------------------------------------------------------
   virtual const std::string& getOperatingSystemName() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the full executable path
   /// \return  	                     the full executable path (path + filename)
   //-----------------------------------------------------------------------------
   virtual const std::string& getExecutablePath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     indicates if the server is fully loaded
   /// \return  	                     true if the server is fully loaded
   //-----------------------------------------------------------------------------
   virtual bool isServerFullyLoaded() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     set the server state to fully loaded
   //-----------------------------------------------------------------------------
   virtual void setServerFullyLoaded() = 0;
};
