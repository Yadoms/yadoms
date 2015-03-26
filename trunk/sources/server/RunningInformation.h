#pragma once

#include "IRunningInformation.h"

//-----------------------------------------------------------------------------
/// \class   Class which handle information about server system
//-----------------------------------------------------------------------------
class CRunningInformation : public IRunningInformation
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		      Constructor
   /// \param [in] path The executable path
   //-----------------------------------------------------------------------------
   CRunningInformation(const std::string & path);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CRunningInformation();

   // IRunningInformation implementation
   virtual const boost::posix_time::ptime  & getStartupDateTime() const;
   virtual const tools::CVersion  & getSoftwareVersion() const;
   virtual const std::string getOperatingSystemName() const;
   virtual const std::string & getExecutablePath() const;
   // [END] IRunningInformation implementation
   
private:
   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms startup date/time
   //-----------------------------------------------------------------------------
   boost::posix_time::ptime   m_startupDateTime;
   
   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms version
   //-----------------------------------------------------------------------------
   tools::CVersion   m_softwareVersion;

   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms executable path
   //-----------------------------------------------------------------------------
   std::string m_executablePath;
};

