#include "stdafx.h"

#include "PgsqlRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/common/adapters/GenericAdapter.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include <shared/StringExtension.h>
#include "PgsqlSystemTables.h"
#include "database/DatabaseException.hpp"
#include "PgsqlResultHandler.h"
#include "PgsqlQuery.h"
#include "PgsqlTableCreationScriptProvider.h"

namespace database { 
namespace pgsql { 


   CPgsqlRequester::CPgsqlRequester(const std::string &host, const unsigned int port, const std::string &dbName, const std::string &login, const std::string &password)
      :m_host(host), m_port(port), m_dbName(dbName), m_login(login), m_password(password), m_bOneTransactionActive(false)
   {
   }

   CPgsqlRequester::~CPgsqlRequester()
   {

   }
   
   PGconn * CPgsqlRequester::createNewConnection()
   {
      //connect to postgresql engine
      PGconn * result = PQconnectdb(createConnectionString().c_str());
      
      //Check to see that the backend connection was successfully made
      if (PQstatus(result) != CONNECTION_OK)
      {
         //save the error message
         std::string error = getLastErrorMessage(result);
         
         //clear connection
         PQfinish(result);
         
         throw CDatabaseException("Fail to connect database", error);
      }
      return result;
   }
   
   void CPgsqlRequester::terminateConnection(PGconn * pConnection)
   {
      if(pConnection != NULL)
      {
         PQfinish(pConnection);
         pConnection = NULL;
      }
   }

   std::string CPgsqlRequester::getLastErrorMessage(PGconn * pConnection)
   {
      return std::string(PQerrorMessage(pConnection));
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
         throw CDatabaseException("Fail to connect database : REJECT : The server is running but is in a state that disallows connections (startup, shutdown, or crash recovery) ");
      case PQPING_NO_RESPONSE:
         throw CDatabaseException("Fail to connect database : NO_RESPONSE : The server could not be contacted. This might indicate that the server is not running, or that there is something wrong with the given connection parameters (for example, wrong port number), or that there is a network connectivity problem (for example, a firewall blocking the connection request) ");
      case PQPING_NO_ATTEMPT:
         throw CDatabaseException("Fail to connect database : PQPING_NO_ATTEMPT : No attempt was made to contact the server, because the supplied parameters were obviously incorrect or there was some client-side problem (for example, out of memory) ");
      }
   }

   // IDatabaseEngine implementation
   database::common::CQuery CPgsqlRequester::newQuery()
   {
      return CPgsqlQuery();
   }

   void CPgsqlRequester::initialize()
   {
      YADOMS_LOG(information) << "Connect to database";
      try
      {
         //connect to postgresql engine
         PGconn * pConnection = PQconnectdb(createConnectionString().c_str());

         //Check to see that the backend connection was successfully made 
         if (PQstatus(pConnection) != CONNECTION_OK)
         {
            //save the error message
            std::string firstError = getLastErrorMessage(pConnection);

            //clear connection
            PQfinish(pConnection);

            YADOMS_LOG(information) << "Fail to connect the database.";

            //ping the server (throws if ping failed)
            YADOMS_LOG(information) << "Check server availability";
            pingServer();
         
            //retry with "postgres" database to be able to list db
            YADOMS_LOG(information) << "Server is available. Checking database existance";
            pConnection = PQconnectdb(createConnectionString(kMasterDb).c_str());
            if (PQstatus(pConnection) != CONNECTION_OK)
            {
               throw CDatabaseException("Fail to connect database", getLastErrorMessage(pConnection));
            }
            else
            {
               //list all databases
               CPgsqlQuery dbList;
               
               dbList.SelectCount().From(CPgDatabaseTable::getTableName()).Where(CPgDatabaseTable::getDatabaseNameColumnName(), CQUERY_OP_ILIKE, m_dbName);
               int count = queryCount(dbList, pConnection);
               if (count == 0)
               {
                  YADOMS_LOG(information) << "Database do not exists, try to create it";
                  //create database
                  int result = queryStatement(CPgsqlQuery().CreateDatabase(m_dbName), true, pConnection);
                  if (result == 0)
                  {
                     YADOMS_LOG(information) << "Database created";

                     //terminate master connection
                     PQfinish(pConnection);
                     
                     //retry connection to database
                     pConnection = PQconnectdb(createConnectionString().c_str());

                     //Check to see that the backend connection was successfully made 
                     if (PQstatus(pConnection) != CONNECTION_OK)
                        throw CDatabaseException("Fail to connect the newly createed database.", getLastErrorMessage(pConnection));
                     
                     //terminate master connection
                     PQfinish(pConnection);
                  }
                  else
                  {
                     throw CDatabaseException("Fail to create database.", getLastErrorMessage(pConnection));
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
         throw;
      }
   }

   void CPgsqlRequester::finalize()
   {
   }

   void CPgsqlRequester::queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute)
   {
      PGconn * pConnection = NULL;
      
      try
      {
         pConnection = createNewConnection();
         queryEntities(pAdapter, querytoExecute, pConnection);
         PQfinish(pConnection);

      } catch (database::CDatabaseException & )
      {
         PQfinish(pConnection);
         throw;
      }
   }
   
   void CPgsqlRequester::queryEntities(database::common::adapters::IResultAdapter * pAdapter, const database::common::CQuery & querytoExecute, PGconn * pConnection)
   {
      BOOST_ASSERT(pAdapter != NULL);

      if (pAdapter != NULL)
      {
         PGresult *res = NULL;
         try
         {

            //execute query
            YADOMS_LOG(debug) << "[REQUEST] queryEntities - " << querytoExecute.str();
            res = PQexec(pConnection, querytoExecute.c_str());

            ExecStatusType resultCode = PQresultStatus(res);
            if (resultCode != PGRES_TUPLES_OK)
            {
               //make a copy of the err message
               std::string errMessage(PQerrorMessage(pConnection));

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
   
   int CPgsqlRequester::queryStatement(const database::common::CQuery & querytoExecute, bool throwIfFails)
   {
      PGconn * pConnection = NULL;
      
      try
      {
         pConnection = createNewConnection();
         int result = queryStatement(querytoExecute, throwIfFails, pConnection);
         PQfinish(pConnection);
         return result;
         
      } catch (database::CDatabaseException &)
      {
         PQfinish(pConnection);
         if(throwIfFails)
            throw;
         return -1;
      }
   }

   int CPgsqlRequester::queryStatement(const database::common::CQuery & querytoExecute, bool throwIfFails, PGconn * pConnection)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kSelect);


      //execute query
      YADOMS_LOG(debug) << "[REQUEST] queryStatement - " << querytoExecute.str();
      PGresult *res = PQexec(pConnection, querytoExecute.c_str());

      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
         //make a copy of the err message
         std::string errMessage(PQerrorMessage(pConnection));
            
         //log the message
         YADOMS_LOG(error) << "Query failed : " << std::endl << "Query: " << querytoExecute.str() << std::endl << "Error : " << errMessage;

         //free memory
         PQclear(res);

         if(throwIfFails)
            throw CDatabaseException(errMessage);
         return -1;
      }
      
      int affectedRows = 0;
      std::string affectedRowsString = PQcmdTuples(res);
      if (!affectedRowsString.empty())
         affectedRows = atoi(affectedRowsString.c_str());
      PQclear(res);
      return affectedRows;
   }

   int CPgsqlRequester::queryCount(const database::common::CQuery & querytoExecute)
   {
      PGconn * pConnection = NULL;
      
      try
      {
         pConnection = createNewConnection();
         int result = queryCount(querytoExecute, pConnection);
         PQfinish(pConnection);
         return result;
         
      } catch (database::CDatabaseException &)
      {
         PQfinish(pConnection);
         throw;
      }
   }
   
   int CPgsqlRequester::queryCount(const database::common::CQuery & querytoExecute, PGconn * pConnection)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CPgsqlQuery::kSelect);

      //execute query
      YADOMS_LOG(debug) << "[REQUEST] queryCount - " << querytoExecute.str();
      PGresult *res = PQexec(pConnection, querytoExecute.c_str());

      if (PQresultStatus(res) != PGRES_TUPLES_OK)
      {
         //make a copy of the err message
         std::string errMessage(PQerrorMessage(pConnection));

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
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CPgsqlQuery::kSelect);

      QueryResults results = query(querytoExecute);
      if(results.size() >= 1)
         return results[0];

      return QueryRow(); //returns empty data
   }

   
   CPgsqlRequester::QueryResults CPgsqlRequester::query(const database::common::CQuery & querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CPgsqlQuery::kSelect);

      database::common::adapters::CGenericAdapter genericAdapter;
      queryEntities(&genericAdapter, querytoExecute);
      return genericAdapter.getResults();
   }


   
   bool CPgsqlRequester::transactionSupport()
   {
      return false;
   }

   void CPgsqlRequester::transactionBegin()
   {
      PGconn * pConnection = NULL;
      
      try
      {
         pConnection = createNewConnection();
         transactionBegin(pConnection);
         PQfinish(pConnection);
      }
      catch (database::CDatabaseException &)
      {
         PQfinish(pConnection);
         throw;
      }
   }
   
   void CPgsqlRequester::transactionBegin(PGconn * pConnection)
   {
      if(!m_bOneTransactionActive)
      {
         PGresult   * res = PQexec(pConnection, "BEGIN");
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_bOneTransactionActive = true;
         }
         else 
         {
            //make a copy of the err message
            std::string errMessage(PQerrorMessage(pConnection));

            //log the message
            YADOMS_LOG(error) << "Fail to start transaction : " << std::endl << "Error : " << errMessage;

         }
         PQclear(res);
      }
   }

   void CPgsqlRequester::transactionCommit()
   {
      PGconn * pConnection = NULL;
      
      try
      {
         pConnection = createNewConnection();
         transactionCommit(pConnection);
         PQfinish(pConnection);
      }
      catch (database::CDatabaseException &)
      {
         PQfinish(pConnection);
         throw;
      }
   }
   
   void CPgsqlRequester::transactionCommit(PGconn * pConnection)
   {
      if (m_bOneTransactionActive)
      {
         PGresult   * res = PQexec(pConnection, "COMMIT");
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_bOneTransactionActive = false;
         }
         else
         {
            //make a copy of the err message
            std::string errMessage(PQerrorMessage(pConnection));

            //log the message
            YADOMS_LOG(error) << "Fail to commit transaction : " << std::endl << "Error : " << errMessage;

         }
         PQclear(res);
      }
   }

   void CPgsqlRequester::transactionRollback()
   {
        PGconn * pConnection = NULL;
         
         try
         {
            pConnection = createNewConnection();
            transactionRollback(pConnection);
            PQfinish(pConnection);
         }
         catch (database::CDatabaseException &)
         {
            PQfinish(pConnection);
            throw;
         }
   
   }
   
   void CPgsqlRequester::transactionRollback(PGconn * pConnection)
   {
      if (m_bOneTransactionActive)
      {
         PGresult   * res = PQexec(pConnection, "ROLLBACK");
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_bOneTransactionActive = false;
         }
         else
         {
            //make a copy of the err message
            std::string errMessage(PQerrorMessage(pConnection));

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
      CPgsqlQuery sCheckForTableExists;
      sCheckForTableExists.SelectExists(CPgsqlQuery().
         Select().
         From(CPgsqlTablesTable::getTableName()).
         Where(CPgsqlTablesTable::getTableColumnName(), CQUERY_OP_ILIKE, tableName));


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
         CPgsqlQuery drop;
         queryStatement(drop.DropTable(tableName));
         return !checkTableExists(tableName);
      }
      return true;
   }


   bool CPgsqlRequester::createTableIfNotExists(const std::string & tableName, const std::string & tableScript)
   {
      if (!checkTableExists(tableName))
      {
         queryStatement(CPgsqlQuery::CustomQuery(tableScript, CPgsqlQuery::kCreate));
         return checkTableExists(tableName);
      }
      return true;
   }

   void CPgsqlRequester::createIndex(const std::string & tableName, const std::string & indexScript)
   {
      queryStatement(CPgsqlQuery::CustomQuery(indexScript, CPgsqlQuery::kCreate));
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
         queryStatement(CPgsqlQuery().Vacuum());
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

