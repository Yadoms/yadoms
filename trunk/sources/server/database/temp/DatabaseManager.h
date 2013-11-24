//
// DatabaseManager.h
//
// yadoms-database manager
//
#pragma once

#include <string>
#include "sqlite3.h"

class CDatabaseManager
{
public:
    //--------------------------------------------------------------
    /// \brief			Constructor
    //--------------------------------------------------------------
    CDatabaseManager();

    //--------------------------------------------------------------
    /// \brief			Destructor
    //--------------------------------------------------------------
    virtual ~CDatabaseManager();

public:
    //--------------------------------------------------------------
    /// \brief			Load the database
    /// \param [in]		databasePath : the path of the db file
    /// \param [in]		databasePassword : the db password
    /// \return    		true if the dabase is loaded with success, else false
    //--------------------------------------------------------------
    bool load(const std::string & databasePath, const std::string & databasePassword);

    //--------------------------------------------------------------
    /// \brief			Unload the database
    //--------------------------------------------------------------
    void unload();

private:
    //--------------------------------------------------------------
    /// \brief			The database handle
    //--------------------------------------------------------------
    sqlite3 * m_pDatabase;

};