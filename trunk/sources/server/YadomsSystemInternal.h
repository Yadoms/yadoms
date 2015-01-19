#pragma once

#include "tools/Version.h"

//-----------------------------------------------------------------------------
/// \class   Class which handle information about server system
//-----------------------------------------------------------------------------
class CYadomsSystemInternal
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		                     Constructor
   //-----------------------------------------------------------------------------
   CYadomsSystemInternal();

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CYadomsSystemInternal();

   //-----------------------------------------------------------------------------
   /// \brief		                     get the startup date/time
   /// \return  	                     the startup date/time
   //-----------------------------------------------------------------------------
   const boost::posix_time::ptime  & getStartupDateTime() const;
   
   //-----------------------------------------------------------------------------
   /// \brief		                     get the software version
   /// \return  	                     the software version
   //-----------------------------------------------------------------------------
   const tools::CVersion  & getSoftwareVersion() const;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the operating system name (windows, linux,....)
   /// \return  	                     the operating system name
   //-----------------------------------------------------------------------------
   const std::string & getOperatingSystemName() const;
   
private:
   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms startup date/time
   //-----------------------------------------------------------------------------
   boost::posix_time::ptime   m_startupDateTime;
   
   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms version
   //-----------------------------------------------------------------------------
   tools::CVersion   m_softwareVersion;
};

