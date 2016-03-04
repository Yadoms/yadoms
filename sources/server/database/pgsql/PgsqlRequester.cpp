#include "stdafx.h"

#include "PgsqlRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/common/adapters/GenericAdapter.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include <shared/StringExtension.h>
#include "PgsqlSystemTables.h"
#include "database/DatabaseException.hpp"
#include "PgsqlResultHandler.h"
#include "Query.h"
#include "PgsqlTableCreationScriptProvider.h"

namespace database { 
namespace pgsql { 


   CPgsqlRequester::CPgsqlRequester(const std::string &host, const unsigned int port, const std::string &dbName, const std::string &login, const std::string &password)
      :m_host(host), m_port(port), m_dbName(dbName), m_login(login), m_password(password), m_pConnection(NULL), m_bOneTransactionActive(false), m_schema("yadoms")
   {
   }

   CPgsqlRequester::~CPgsqlRequester()
   {
      terminateConnection();
   }
   void CPgsqlRequester::terminateConnection()
   {
      if (m_pConnection != NULL)
      {
         PQfinish(m_pConnection);
         m_pConnection = NULL;
      }
   }

   std::string CPgsqlRequester::getLastErrorMessage()
   {
      return std::string(PQerrorMessage(m_pConnection));
   }

   const std::string CPgsqlRequester::createConnectionString(const EConnectionStringMode mode)
   {
      switch (mode)
      {
      case kPing:
         return (boost::format("host=%1% port=%2% application_name=Yadoms") % m_host % m_port).str();
      case kMasterDb:
         return (boost::format("host=%1% port=%2% user=%3% password=%4% dbname=postgres application_name=Yadoms") % m_host % m_port % m_login % m_password).str();
      case kNormal:
      default:
         return (boost::format("host=%1% port=%2% user=%3% password=%4% dbname=%5% application_name=Yadoms") % m_host % m_port % m_login % m_password % m_dbName).str();
      }
   }

   void CPgsqlRequester::pingServer()
   {
      //fails to connect database
      PGPing serverStatus = PQping(createConnectionString(kPing).c_str());
      switch (serverStatus)
      {
      case PQPING_REJECT:
         throw CDatabaseException("Fail to connect database : REJECT : The server is running but is in a state that disallows connections (startup, shutdown, or crash recovery) " , getLastErrorMessage());
      case PQPING_NO_RESPONSE:
         throw CDatabaseException("Fail to connect database : NO_RESPONSE : The server could not be contacted. This might indicate that the server is not running, or that there is something wrong with the given connection parameters (for example, wrong port number), or that there is a network connectivity problem (for example, a firewall blocking the connection request) " , getLastErrorMessage());
      case PQPING_NO_ATTEMPT:
         throw CDatabaseException("Fail to connect database : PQPING_NO_ATTEMPT : No attempt was made to contact the server, because the supplied parameters were obviously incorrect or there was some client-side problem (for example, out of memory) " , getLastErrorMessage());
      }
   }

   // IDatabaseEngine implementation
   void CPgsqlRequester::initialize()
   {
      YADOMS_LOG(information) << "Connect to database";
      try
      {
         //connect to postgresql engine
         m_pConnection = PQconnectdb(createConnectionString().c_str());

         //Check to see that the backend connection was successfully made 
         if (PQstatus(m_pConnection) != CONNECTION_OK)
         {
            //save the error message
            std::string firstError = getLastErrorMessage();

            //clear connection
            PQfinish(m_pConnection);

            YADOMS_LOG(information) << "Fail to connect the database.";

            //ping the server (throws if ping failed)
            YADOMS_LOG(information) << "Check server availability";
            pingServer();
         
            //retry with "postgres" database to be able to list db
            YADOMS_LOG(information) << "Server is available. Checking database existance";
            m_pConnection = PQconnectdb(createConnectionString(kMasterDb).c_str());
            if (PQstatus(m_pConnection) != CONNECTION_OK)
            {
               throw CDatabaseException("Fail to connect database", getLastErrorMessage());
            }
            else
            {
               //list all databases
               CQuery dbList;
               
               dbList.SelectCount().From(CPgDatabaseTable::getTableName()).Where(CPgDatabaseTable::getDatabaseNameColumnName(), CQUERY_OP_EQUAL, m_dbName);
               int count = queryCount(dbList);
               if (count == 0)
               {
                  YADOMS_LOG(information) << "Database do not exists, try to create it";
                  //create database
                  int result = queryStatement(CQuery().CreateDatabase(m_dbName));
                  if (result == 0)
                  {
                     YADOMS_LOG(information) << "Database created";

                     //terminate master connection
                     PQfinish(m_pConnection);
                     
                     //retry connection to database
                     m_pConnection = PQconnectdb(createConnectionString().c_str());

                     //Check to see that the backend connection was successfully made 
                     if (PQstatus(m_pConnection) != CONNECTION_OK)
                        throw CDatabaseException("Fail to connect the newly createed database.", getLastErrorMessage());
                  }
                  else
                  {
                     throw CDatabaseException("Fail to create database.", getLastErrorMessage());
                  }
               }
               else
               {
                  throw CDatabaseException("Database exists, but connection fails. (credentials for database ?)", firstError);
               }
            }
         }
      }
      catch (...)
      {
         terminateConnection();
         throw;
      }
   }

   void CPgsqlRequester::finalize()
   {
      //close database access
      terminateConnection();
   }

   void CPgsqlRequester::queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute)
   {
      BOOST_ASSERT(pAdapter != NULL);

      if (pAdapter != NULL)
      {
         PGresult *res = NULL;
         try
         {
            //execute query
            res = PQexec(m_pConnection, querytoExecute.c_str());

            ExecStatusType resultCode = PQresultStatus(res);
            if (resultCode != PGRES_TUPLES_OK)
            {
               //make a copy of the err message
               std::string errMessage(PQerrorMessage(m_pConnection));

               //log the message
               YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << errMessage;

               //free memory
               PQclear(res);
               res = NULL;

               //throw
               throw CDatabaseException(errMessage);
            }


            boost::shared_ptr<CPgsqlResultHandler> handler(new CPgsqlResultHandler(res));
            if (!pAdapter->adapt(handler))
            {
               YADOMS_LOG(error) << "Fail to adapt values";
            }
         }
         catch (std::exception &ex)
         {
            YADOMS_LOG(error) << "Exception: Fail to execute query : " << ex.what();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown exception: Fail to execute query";
         }

         //free memory
         if (res != NULL)
         {
            PQclear(res);
            res = NULL;
         }
      }
      else
      {
         //throw exception
         throw shared::exception::CNullReference("pAdapter");
      }
   }


   int CPgsqlRequester::queryStatement(const database::common::CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kSelect);


      //execute query
      PGresult *res = PQexec(m_pConnection, querytoExecute.c_str());

      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
         //make a copy of the err message
         std::string errMessage(PQerrorMessage(m_pConnection));
            
         //log the message
         YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << errMessage;

         //free memory
         PQclear(res);

         //throw
         throw CDatabaseException(errMessage);
      }
      
      int affectedRows = PQntuples(res);
      PQclear(res);
      return affectedRows;
   }


   int CPgsqlRequester::queryCount(const database::common::CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CQuery::kSelect);

      //execute query
      PGresult *res = PQexec(m_pConnection, querytoExecute.c_str());

      if (PQresultStatus(res) != PGRES_TUPLES_OK)
      {
         //make a copy of the err message
         std::string errMessage(PQerrorMessage(m_pConnection));

         //log the message
         YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << errMessage;

         //free memory
         PQclear(res);

         //throw
         throw CDatabaseException(errMessage);
      }

      CPgsqlResultHandler handler(res);
      handler.next_step();
      return handler.extractValueAsInt(0);
   }

   CPgsqlRequester::QueryRow CPgsqlRequester::querySingleLine(const database::common::CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CQuery::kSelect);

      QueryResults results = query(querytoExecute);
      if(results.size() >= 1)
         return results[0];

      return QueryRow(); //returns empty data
   }


   CPgsqlRequester::QueryResults CPgsqlRequester::query(const database::common::CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CQuery::kSelect);

      database::common::adapters::CGenericAdapter genericAdapter;
      queryEntities(&genericAdapter, querytoExecute);
      return genericAdapter.getResults();
   }

   std::string CPgsqlRequester::generateSqlIsoDateFormat(const std::string &columnName)
   {
      return "to_char(" + columnName + ", 'YYYY\"-\"MM\"-\"DD\"T\"HH24\":\"MI\":\"SS')";
   }


   bool CPgsqlRequester::transactionSupport()
   {
      return true;
   }


   void CPgsqlRequester::transactionBegin()
   {
      if(!m_bOneTransactionActive)
      {
         PGresult   * res = PQexec(m_pConnection, "BEGIN");
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_bOneTransactionActive = true;
         }
         else 
         {
            //make a copy of the err message
            std::string errMessage(PQerrorMessage(m_pConnection));

            //log the message
            YADOMS_LOG(error) << "Fail to start transaction : " << std::endl << "Error : " << errMessage;

         }
         PQclear(res);
      }
   }

   void CPgsqlRequester::transactionCommit()
   {
      if (m_bOneTransactionActive)
      {
         PGresult   * res = PQexec(m_pConnection, "COMMIT");
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_bOneTransactionActive = false;
         }
         else
         {
            //make a copy of the err message
            std::string errMessage(PQerrorMessage(m_pConnection));

            //log the message
            YADOMS_LOG(error) << "Fail to commit transaction : " << std::endl << "Error : " << errMessage;

         }
         PQclear(res);
      }
   }

   void CPgsqlRequester::transactionRollback()
   {
      if (m_bOneTransactionActive)
      {
         PGresult   * res = PQexec(m_pConnection, "ROLLBACK");
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_bOneTransactionActive = false;
         }
         else
         {
            //make a copy of the err message
            std::string errMessage(PQerrorMessage(m_pConnection));

            //log the message
            YADOMS_LOG(error) << "Fail to rollback transaction : " << std::endl << "Error : " << errMessage;

         }
         PQclear(res);
      }
   }

   bool CPgsqlRequester::transactionIsAlreadyCreated()
   {
      return m_bOneTransactionActive;
   }

   bool CPgsqlRequester::checkTableExists(const std::string & tableName)
   {
      //check that table Configuration exists
      CQuery sCheckForTableExists;
      sCheckForTableExists.SelectExists(CQuery().
         Select().
         From(CPgsqlTablesTable::getTableName()).
         Where(CPgsqlTablesTable::getSchemaColumnName(), CQUERY_OP_EQUAL, "yadoms")
         .And(CPgsqlTablesTable::getTableColumnName(), CQUERY_OP_EQUAL, tableName));


      database::common::adapters::CSingleValueAdapter<bool> existsAdapter;
      queryEntities(&existsAdapter, sCheckForTableExists);
      if (existsAdapter.getResults().size() >= 1)
         return existsAdapter.getResults()[0];
      return false;
   }


   bool CPgsqlRequester::dropTableIfExists(const std::string & tableName)
   {
      if(checkTableExists(tableName))
      {
         CQuery drop;
         queryStatement(drop.DropTable(tableName));
         return !checkTableExists(tableName);
      }
      return true;
   }


   bool CPgsqlRequester::createTableIfNotExists(const std::string & tableName, const std::string & tableScript)
   {
      if (!checkTableExists(tableName))
      {
         queryStatement(CQuery::CustomQuery(tableScript, CQuery::kCreate));
         return checkTableExists(tableName);
      }
      return true;
   }

   void CPgsqlRequester::createIndex(const std::string & tableName, const std::string & indexScript)
   {
      queryStatement(CQuery::CustomQuery(indexScript, CQuery::kCreate));
   }

   void CPgsqlRequester::vacuum()
   {
      //we ensure that no transaction is active
      //is a transaction is active, hust wait for the transaction to end (timetout 20sec)
      int waitLoopCount = 0;
      int maxLoopWait = 600; //2 min

      while(m_bOneTransactionActive && waitLoopCount < maxLoopWait)
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(200));
         waitLoopCount++;
      }

      //if no timeout, execute vacuum
      if (waitLoopCount >= maxLoopWait || m_bOneTransactionActive)
         YADOMS_LOG(warning) << "Fail to execute vacuum, one transaction is still active";
      else
         queryStatement(CQuery().Vacuum());
   }

   boost::shared_ptr<ITableCreationScriptProvider> CPgsqlRequester::getTableCreationScriptProvider()
   {
      return boost::shared_ptr<ITableCreationScriptProvider>(new CPgsqlTableCreationScriptProvider());
   }

   bool CPgsqlRequester::backupSupported()
   {
      return false;
   }

   void CPgsqlRequester::backupData(const std::string & backupLocation, IDataBackup::ProgressFunc reporter)
   {
      throw database::CDatabaseException("Unsupported backup for PostgreSQL");
   }


} //namespace pgsql
} //namespace database 

