#include "stdafx.h"
#include "PathProvider.h"

CPathProvider::CPathProvider(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   : m_logsPath("logs"),
     m_pluginsLogPath(m_logsPath / "plugins"),
     m_scriptsLogPath(m_logsPath / "scripts"),
     m_pluginsPath(startupOptions->getPluginsPath()),
     m_scriptsPath("scripts"),
     m_scriptInterpreters(startupOptions->getScriptInterpretersPath()),
     m_databaseFile(startupOptions->getDatabaseFile())
{
   if (!boost::filesystem::exists(m_logsPath))
      boost::filesystem::create_directory(m_logsPath);
   if (!boost::filesystem::exists(m_pluginsLogPath))
      boost::filesystem::create_directory(m_pluginsLogPath);
   if (!boost::filesystem::exists(m_scriptsLogPath))
      boost::filesystem::create_directory(m_scriptsLogPath);
   if (!boost::filesystem::exists(m_pluginsPath))
      boost::filesystem::create_directory(m_pluginsPath);
   if (!boost::filesystem::exists(m_scriptsPath))
      boost::filesystem::create_directory(m_scriptsPath);
   if (!boost::filesystem::exists(m_scriptInterpreters))
      boost::filesystem::create_directory(m_scriptInterpreters);
}

CPathProvider::~CPathProvider()
{
}


const boost::filesystem::path& CPathProvider::logsPath() const
{
   return m_logsPath;
}

const boost::filesystem::path& CPathProvider::pluginsLogPath() const
{
   return m_pluginsLogPath;
}

const boost::filesystem::path& CPathProvider::pluginsPath() const
{
   return m_pluginsPath;
}
const boost::filesystem::path& CPathProvider::scriptsPath() const
{
   return m_scriptsPath;
}

const boost::filesystem::path& CPathProvider::scriptsLogPath() const
{
   return m_scriptsLogPath;
}

const boost::filesystem::path& CPathProvider::scriptInterpretersPath() const
{
   return m_scriptInterpreters;
}

const boost::filesystem::path& CPathProvider::getDatabaseFile() const
{
   return m_databaseFile;
}