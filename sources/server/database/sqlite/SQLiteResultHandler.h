#pragma once

#include "sqlite3.h"
#include "database/common/IResultHandler.h"

namespace database { 
namespace sqlite { 

   class CSQLiteResultHandler : public database::common::IResultHandler
   {
   public:
      explicit CSQLiteResultHandler(sqlite3_stmt *pStatement);
      virtual ~CSQLiteResultHandler();

      // database::common::IResultHandler implementation
      int getColumnCount() override;
      std::string getColumnName(const int columnIndex) override;
      bool next_step() override;
      std::string extractValueAsString(const int columnIndex) override;
      int extractValueAsInt(const int columnIndex) override;
      float extractValueAsFloat(const int columnIndex) override;
      double extractValueAsDouble(const int columnIndex) override;
      unsigned char* extractValueAsBlob(const int columnIndex) override;
      bool extractValueAsBool(const int columnIndex) override;
      bool isValueNull(const int columnIndex) override;
      boost::posix_time::ptime extractValueAsBoostTime(const int columnIndex) override;
      Poco::DateTime extractValueAsPocoTime(const int columnIndex) override;
      shared::CDataContainer extractValueAsDataContainer(const int columnIndex) override;
      // [END] - database::common::IResultHandler implementation

   private:
      //--------------------------------------------------------------
      /// \Brief		database statement
      //--------------------------------------------------------------
      sqlite3_stmt *m_pStatement;

   };

} //namespace sqlite
} //namespace database 

