#pragma once
#include "IRunningInformation.h"

//-----------------------------------------------------------------------------
/// \brief   Class which handle information about server system
//-----------------------------------------------------------------------------
class CRunningInformation : public IRunningInformation
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		      Constructor
   /// \param [in] path The executable path
   /// \param [in] yadomsVersion The Yadoms version
   //-----------------------------------------------------------------------------
   explicit CRunningInformation(const std::string& path,
                                const shared::versioning::CSemVer& yadomsVersion);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CRunningInformation() = default;

   // IRunningInformation implementation
   const boost::posix_time::ptime& getStartupDateTime() const override;
   const shared::versioning::CVersionInformation& getSoftwareVersion() const override;
   const std::string& getOperatingSystemName() const override;
   const std::string& getExecutablePath() const override;
   bool isServerFullyLoaded() const override;
   void setServerFullyLoaded() override;
   // [END] IRunningInformation implementation

private:
   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms startup date/time
   //-----------------------------------------------------------------------------
   boost::posix_time::ptime m_startupDateTime;

   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms version
   //-----------------------------------------------------------------------------
   const shared::versioning::CVersionInformation m_softwareVersion;

   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms executable path
   //-----------------------------------------------------------------------------
   std::string m_executablePath;

   //-----------------------------------------------------------------------------
   /// \brief		                     Yadoms server state
   //-----------------------------------------------------------------------------
   bool m_serverFullyLoaded;
};
