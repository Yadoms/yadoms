#include "DatabaseManager.h"
#include <boost/log/trivial.hpp>

CDatabaseManager::CDatabaseManager()
    :m_pDatabase(NULL)
{
}

CDatabaseManager::~CDatabaseManager()
{
    BOOST_ASSERT(m_pDatabase);
}

bool CDatabaseManager::load(const std::string & databasePath, const std::string & databasePassword)
{
    BOOST_LOG_TRIVIAL(info) << "Load database";

    bool result = false;

    try
    {
        int rc = sqlite3_open(databasePath.c_str(), &m_pDatabase);
        if(rc)
        {
            BOOST_LOG_TRIVIAL(fatal) << "Fail to load database: " << sqlite3_errmsg(m_pDatabase);
            sqlite3_close(m_pDatabase);
            result = false;
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << "Load database with success";
            result = true;
        }
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << "Unknow exception while loading database";
        sqlite3_close(m_pDatabase);
        result = false;
    }
    return result;
}


void CDatabaseManager::unload()
{
    sqlite3_close(m_pDatabase);
}
