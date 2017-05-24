#pragma once

#include "libpq-fe.h"
#include "database/common/IResultHandler.h"

namespace database { namespace pgsql {

   class CPgsqlResultHandler : public database::common::IResultHandler
   {
   public:
      explicit CPgsqlResultHandler(PGresult *res);
      virtual ~CPgsqlResultHandler();

      // database::common::IResultHandler implementation
      virtual int getColumnCount();
      virtual std::string getColumnName(const int columnIndex);
      virtual bool next_step();
      virtual std::string extractValueAsString(const int columnIndex);
      virtual int extractValueAsInt(const int columnIndex);
      virtual float extractValueAsFloat(const int columnIndex);
      virtual double extractValueAsDouble(const int columnIndex);
      virtual unsigned char* extractValueAsBlob(const int columnIndex);
      virtual bool extractValueAsBool(const int columnIndex);
      virtual bool isValueNull(const int columnIndex);
      virtual boost::posix_time::ptime extractValueAsBoostTime(const int columnIndex);
      virtual Poco::DateTime extractValueAsPocoTime(const int columnIndex);
      virtual shared::CDataContainer extractValueAsDataContainer(const int columnIndex);
      // [END] - database::common::IResultHandler implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		database statement
      //--------------------------------------------------------------
      PGresult *m_res;

      //--------------------------------------------------------------
      /// \Brief		Current result index (n° row)
      //--------------------------------------------------------------
      int m_currentResultIndex;     
      
      //--------------------------------------------------------------
      /// \Brief		Current result row count
      //--------------------------------------------------------------
      int m_currentResultRowCount;
   };

} //namespace pgsql
} //namespace database 

