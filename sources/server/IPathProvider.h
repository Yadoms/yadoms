#pragma once

//-----------------------------------------------------------------------------
/// \biref   Interface on Yadoms paths provider
//-----------------------------------------------------------------------------
class IPathProvider
{
public:
   virtual ~IPathProvider() {}

   //-----------------------------------------------------------------------------
   /// \brief		                     get the logs path
   /// \return  	                     the logs path, ususally {yadoms_binary_path}/logs
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& logsPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the web server path
   /// \return  	                     the web server path, ususally {yadoms_binary_path}/www
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& webServerPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the plugins path
   /// \return  	                     the plugins path, ususally {yadoms_binary_path}/plugins
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& pluginsPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the logs of plugins path
   /// \return  	                     the logs of plugins path, ususally {yadoms_binary_path}/logs/plugins
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& pluginsLogPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the data of plugins path
   /// \return  	                     the plugins data path, ususally {yadoms_binary_path}/data/plugins
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& pluginsDataPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the scripts path
   /// \return  	                     the scripts path, ususally {yadoms_binary_path}/scripts
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& scriptsPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the logs of scripts path
   /// \return  	                     the logs of scripts path, ususally {yadoms_binary_path}/logs/scripts
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& scriptsLogPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the script interpreters path
   /// \return  	                     the script interpreters path, ususally {yadoms_binary_path}/scriptInterpreters
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& scriptInterpretersPath() const = 0;

   //-----------------------------------------------------------------------------
   /// \brief		                     get the logs of script interpreters path
   /// \return  	                     the logs of script interpreters path, ususally {yadoms_binary_path}/logs/scriptInterpreters
   //-----------------------------------------------------------------------------
   virtual const boost::filesystem::path& scriptInterpretersLogPath() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the path of the SQLite database file
   /// \return     Database file path
   /// \note       If not exist, Yadoms will create the file
   //--------------------------------------------------------------
   virtual const boost::filesystem::path& databaseSqliteFile() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the path of the SQLite database backup file
   /// \return     Database file path
   /// \note       If not exist, Yadoms will create the backup file of overwrite it
   //--------------------------------------------------------------
   virtual const boost::filesystem::path& databaseSqliteBackupFile() const = 0;
};

