#pragma once

#include "sqlite3.h"
#include "database/common/IResultHandler.h"

namespace database
{
   namespace sqlite
   {
      class CSQLiteResultHandler final : public common::IResultHandler
      {
      public:
         explicit CSQLiteResultHandler(sqlite3_stmt* pStatement);
         virtual ~CSQLiteResultHandler() = default;

         // database::common::IResultHandler implementation
         int getColumnCount() override;
         std::string getColumnName(int columnIndex) override;
         bool nextStep() override;
         std::string extractValueAsString(int columnIndex) override;
         int extractValueAsInt(int columnIndex) override;
         float extractValueAsFloat(int columnIndex) override;
         double extractValueAsDouble(int columnIndex) override;
         const unsigned char* extractValueAsBlob(int columnIndex) override;
         bool extractValueAsBool(int columnIndex) override;
         bool isValueNull(int columnIndex) override;
         boost::posix_time::ptime extractValueAsBoostTime(int columnIndex) override;
         Poco::DateTime extractValueAsPocoTime(int columnIndex) override;
         boost::shared_ptr<shared::CDataContainer> extractValueAsDataContainer(int columnIndex) override;
         // [END] - database::common::IResultHandler implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		database statement
         //--------------------------------------------------------------
         sqlite3_stmt* m_pStatement;
      };
   } //namespace sqlite
} //namespace database 
