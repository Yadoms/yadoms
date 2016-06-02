#pragma once

//-----------------------------------------------------------------------------
/// \class   Interface on Yadoms paths provider
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
};

