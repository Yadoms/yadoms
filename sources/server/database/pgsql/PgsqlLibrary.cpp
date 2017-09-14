#include "stdafx.h"
#include "PgsqlLibrary.h"
#include <shared/DynamicLibrary.h>

namespace database
{
   namespace pgsql
   {
      // PostgreSql library are named as :
      // - libpq.dll under Windows
      // - libpq.so under Linux
      // - libpq.dylib under Mac
      CPgsqlLibrary::CPgsqlLibrary()
         : m_lib(std::string("libpq") + shared::CDynamicLibrary::DotExtension(), boost::dll::load_mode::append_decorations)
      {
         loadSymbols();
      }

      CPgsqlLibrary::~CPgsqlLibrary()
      {
      }

      PGconn* CPgsqlLibrary::PQconnectdb(const char* conninfo) const
      {
         return m_PQconnectdbFct(conninfo);
      }

      ConnStatusType CPgsqlLibrary::PQstatus(const PGconn* conn) const
      {
         return m_PQstatusFct(conn);
      }

      void CPgsqlLibrary::PQfinish(PGconn* conn) const
      {
         m_PQfinishFct(conn);
      }

      char* CPgsqlLibrary::PQerrorMessage(const PGconn* conn) const
      {
         return m_PQerrorMessageFct(conn);
      }

      PGPing CPgsqlLibrary::PQping(const char* conninfo) const
      {
         return m_PQpingFct(conninfo);
      }

      int CPgsqlLibrary::PQserverVersion(const PGconn* conn) const
      {
         return m_PQserverVersionFct(conn);
      }

      char* CPgsqlLibrary::PQhost(const PGconn* conn) const
      {
         return m_PQhostFct(conn);
      }

      char* CPgsqlLibrary::PQport(const PGconn* conn) const
      {
         return m_PQportFct(conn);
      }

      char* CPgsqlLibrary::PQdb(const PGconn* conn) const
      {
         return m_PQdbFct(conn);
      }

      char* CPgsqlLibrary::PQuser(const PGconn* conn) const
      {
         return m_PQuserFct(conn);
      }

      void* CPgsqlLibrary::PQgetssl(PGconn* conn) const
      {
         return m_PQgetsslFct(conn);
      }

      PGresult* CPgsqlLibrary::PQexec(PGconn* conn, const char* query) const
      {
         return m_PQexecFct(conn, query);
      }

      ExecStatusType CPgsqlLibrary::PQresultStatus(const PGresult* res) const
      {
         return m_PQresultStatusFct(res);
      }

      char* CPgsqlLibrary::PQresultErrorField(const PGresult* res, int fieldcode) const
      {
         return m_PQresultErrorFieldFct(res, fieldcode);
      }

      void CPgsqlLibrary::PQclear(PGresult* res) const
      {
         m_PQclearFct(res);
      }

      char* CPgsqlLibrary::PQcmdTuples(PGresult* res) const
      {
         return m_PQcmdTuplesFct(res);
      }

      int CPgsqlLibrary::PQntuples(const PGresult* res) const
      {
         return m_PQntuplesFct(res);
      }

      int CPgsqlLibrary::PQnfields(const PGresult* res) const
      {
         return m_PQnfieldsFct(res);
      }

      char* CPgsqlLibrary::PQfname(const PGresult* res, int field_num) const
      {
         return m_PQfnameFct(res, field_num);
      }

      char* CPgsqlLibrary::PQgetvalue(const PGresult* res, int tup_num, int field_num) const
      {
         return m_PQgetvalueFct(res, tup_num, field_num);
      }

      int CPgsqlLibrary::PQbinaryTuples(const PGresult* res) const
      {
         return m_PQbinaryTuplesFct(res);
      }

      int CPgsqlLibrary::PQgetisnull(const PGresult* res, int tup_num, int field_num) const
      {
         return m_PQgetisnullFct(res, tup_num, field_num);
      }

      int CPgsqlLibrary::PQfsize(const PGresult* res, int field_num) const
      {
         return m_PQfsizeFct(res, field_num);
      }

      void CPgsqlLibrary::loadSymbols()
      {
         m_PQconnectdbFct = m_lib.get<PQconnectdbFctType>("PQconnectdb");
         m_PQstatusFct = m_lib.get<PQstatusFctType>("PQstatus");
         m_PQfinishFct = m_lib.get<PQfinishFctType>("PQfinish");
         m_PQerrorMessageFct = m_lib.get<PQerrorMessageFctType>("PQerrorMessage");
         m_PQpingFct = m_lib.get<PQpingFctType>("PQping");
         m_PQserverVersionFct = m_lib.get<PQserverVersionFctType>("PQserverVersion");
         m_PQhostFct = m_lib.get<PQhostFctType>("PQhost");
         m_PQportFct = m_lib.get<PQportFctType>("PQport");
         m_PQdbFct = m_lib.get<PQdbFctType>("PQdb");
         m_PQuserFct = m_lib.get<PQuserFctType>("PQuser");
         m_PQgetsslFct = m_lib.get<PQgetsslFctType>("PQgetssl");
         m_PQexecFct = m_lib.get<PQexecFctType>("PQexec");
         m_PQresultStatusFct = m_lib.get<PQresultStatusFctType>("PQresultStatus");
         m_PQresultErrorFieldFct = m_lib.get<PQresultErrorFieldFctType>("PQresultErrorField");
         m_PQclearFct = m_lib.get<PQclearFctType>("PQclear");
         m_PQcmdTuplesFct = m_lib.get<PQcmdTuplesFctType>("PQcmdTuples");
         m_PQntuplesFct = m_lib.get<PQntuplesFctType>("PQntuples");
         m_PQnfieldsFct = m_lib.get<PQnfieldsFctType>("PQnfields");
         m_PQfnameFct = m_lib.get<PQfnameFctType>("PQfname");
         m_PQgetvalueFct = m_lib.get<PQgetvalueFctType>("PQgetvalue");
         m_PQbinaryTuplesFct = m_lib.get<PQbinaryTuplesFctType>("PQbinaryTuples");
         m_PQgetisnullFct = m_lib.get<PQgetisnullFctType>("PQgetisnull");
         m_PQfsizeFct = m_lib.get<PQfsizeType>("PQfsize");
      }
   } //namespace pgsql
} //namespace database 


