#pragma once
#include "IPathProvider.h"
#include "startupOptions/IStartupOptions.h"

//-----------------------------------------------------------------------------
/// \class   Class which handle information about server system
//-----------------------------------------------------------------------------
class CPathProvider : public IPathProvider
{
public:
   //-----------------------------------------------------------------------------
   /// \brief		                  Constructor
   /// \param[in] startupOptions    Startup options
   /// \note                        Also create needed base folders if not exist (logs, plugins...)
   //-----------------------------------------------------------------------------
   explicit CPathProvider(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

   //-----------------------------------------------------------------------------
   /// \brief		                     Destructor
   //-----------------------------------------------------------------------------
   virtual ~CPathProvider();

   // IPathProvider implementation
   const boost::filesystem::path& logsPath() const override;
   const boost::filesystem::path& webServerPath() const override;
   const boost::filesystem::path& pluginsLogPath() const override;
   const boost::filesystem::path& pluginsDataPath() const override;
   const boost::filesystem::path& pluginsPath() const override;
   const boost::filesystem::path& scriptsPath() const override;
   const boost::filesystem::path& scriptsLogPath() const override;
   const boost::filesystem::path& scriptInterpretersPath() const override;
   const boost::filesystem::path& scriptInterpretersLogPath() const override;
   const boost::filesystem::path& databaseSqliteFile() const override;
   const boost::filesystem::path& backupPath() const override;
   // [END] IPathProvider implementation

private:
   const boost::filesystem::path m_logsPath;
   const boost::filesystem::path m_dataPath;
   const boost::filesystem::path m_webServerPath;
   const boost::filesystem::path m_pluginsLogPath;
   const boost::filesystem::path m_pluginsDataPath;
   const boost::filesystem::path m_scriptsLogPath;

   const boost::filesystem::path m_pluginsPath;

   const boost::filesystem::path m_scriptsPath;

   const boost::filesystem::path m_scriptInterpretersPath;
   const boost::filesystem::path m_scriptInterpretersLogPath;

   const boost::filesystem::path m_databaseSqliteFile;
   const boost::filesystem::path m_backupPath;
};

