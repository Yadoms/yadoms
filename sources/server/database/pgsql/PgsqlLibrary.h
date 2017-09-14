#pragma once
#include <libpq-fe.h>
#include <boost/dll/shared_library.hpp>

namespace database
{
   namespace pgsql
   {
      class CPgsqlLibrary
      {
      public:
         CPgsqlLibrary();
         virtual ~CPgsqlLibrary();

         // Imported functions from offical PostgreSql library
         PGconn* PQconnectdb(const char* conninfo) const;
         ConnStatusType PQstatus(const PGconn* conn) const;
         void PQfinish(PGconn* conn) const;
         char* PQerrorMessage(const PGconn* conn) const;
         PGPing PQping(const char* conninfo) const;
         int PQserverVersion(const PGconn* conn) const;
         char* PQhost(const PGconn* conn) const;
         char* PQport(const PGconn* conn) const;
         char* PQdb(const PGconn* conn) const;
         char* PQuser(const PGconn* conn) const;
         void* PQgetssl(PGconn* conn) const;
         PGresult* PQexec(PGconn* conn, const char* query) const;
         ExecStatusType PQresultStatus(const PGresult* res) const;
         char* PQresultErrorField(const PGresult* res, int fieldcode) const;
         void PQclear(PGresult* res) const;
         char* PQcmdTuples(PGresult* res) const;
         int PQntuples(const PGresult* res) const;
         int PQnfields(const PGresult* res) const;
         char* PQfname(const PGresult* res, int field_num) const;
         char* PQgetvalue(const PGresult* res, int tup_num, int field_num) const;
         int PQbinaryTuples(const PGresult* res) const;
         int PQgetisnull(const PGresult* res, int tup_num, int field_num) const;
         int PQfsize(const PGresult* res, int field_num) const;
         // [END] Imported functions from offical PostgreSql library

      private:
         void loadSymbols();

         boost::dll::shared_library m_lib;

         // PostgreSql library Functions
         typedef PGconn* (*PQconnectdbFctType)(const char* conninfo);
         PQconnectdbFctType m_PQconnectdbFct;

         typedef ConnStatusType (*PQstatusFctType)(const PGconn* conn);
         PQstatusFctType m_PQstatusFct;

         typedef void (*PQfinishFctType)(PGconn* conn);
         PQfinishFctType m_PQfinishFct;

         typedef char* (*PQerrorMessageFctType)(const PGconn* conn);
         PQerrorMessageFctType m_PQerrorMessageFct;

         typedef PGPing (*PQpingFctType)(const char* conninfo);
         PQpingFctType m_PQpingFct;

         typedef int (*PQserverVersionFctType)(const PGconn* conn);
         PQserverVersionFctType m_PQserverVersionFct;

         typedef char* (*PQhostFctType)(const PGconn* conn);
         PQhostFctType m_PQhostFct;

         typedef char* (*PQportFctType)(const PGconn* conn);
         PQportFctType m_PQportFct;

         typedef char* (*PQdbFctType)(const PGconn* conn);
         PQdbFctType m_PQdbFct;

         typedef char* (*PQuserFctType)(const PGconn* conn);
         PQuserFctType m_PQuserFct;

         typedef void* (*PQgetsslFctType)(PGconn* conn);
         PQgetsslFctType m_PQgetsslFct;

         typedef PGresult* (*PQexecFctType)(PGconn* conn, const char* query);
         PQexecFctType m_PQexecFct;

         typedef ExecStatusType (*PQresultStatusFctType)(const PGresult* res);
         PQresultStatusFctType m_PQresultStatusFct;

         typedef char* (*PQresultErrorFieldFctType)(const PGresult* res, int fieldcode);
         PQresultErrorFieldFctType m_PQresultErrorFieldFct;

         typedef void (*PQclearFctType)(PGresult* res);
         PQclearFctType m_PQclearFct;

         typedef char* (*PQcmdTuplesFctType)(PGresult* res);
         PQcmdTuplesFctType m_PQcmdTuplesFct;

         typedef int (*PQntuplesFctType)(const PGresult* res);
         PQntuplesFctType m_PQntuplesFct;

         typedef int (*PQnfieldsFctType)(const PGresult* res);
         PQnfieldsFctType m_PQnfieldsFct;

         typedef char* (*PQfnameFctType)(const PGresult* res, int field_num);
         PQfnameFctType m_PQfnameFct;

         typedef char* (*PQgetvalueFctType)(const PGresult* res, int tup_num, int field_num);
         PQgetvalueFctType m_PQgetvalueFct;

         typedef int (*PQbinaryTuplesFctType)(const PGresult* res);
         PQbinaryTuplesFctType m_PQbinaryTuplesFct;

         typedef int (*PQgetisnullFctType)(const PGresult* res, int tup_num, int field_num);
         PQgetisnullFctType m_PQgetisnullFct;

         typedef int (*PQfsizeType)(const PGresult* res, int field_num);
         PQfsizeType m_PQfsizeFct;
         // [END] PostgreSql library Functions
      };
   } //namespace pgsql
} //namespace database 


