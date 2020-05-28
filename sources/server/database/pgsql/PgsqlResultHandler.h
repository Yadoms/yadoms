#pragma once

#include "database/common/IResultHandler.h"
#include "PgsqlLibrary.h"

namespace database
{
   namespace pgsql
   {
      class CPgsqlResultHandler : public common::IResultHandler
      {
      public:
         CPgsqlResultHandler(boost::shared_ptr<CPgsqlLibrary> pgsqlLibrary, PGresult* res);
         virtual ~CPgsqlResultHandler();

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
         boost::shared_ptr<shared::CDataContainer> extractValueAsDataContainer(const int columnIndex) override;
         // [END] - database::common::IResultHandler implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		Dynamic loaded library instance
         //--------------------------------------------------------------
         boost::shared_ptr<CPgsqlLibrary> m_pgsqlLibrary;

         //--------------------------------------------------------------
         /// \Brief		database statement
         //--------------------------------------------------------------
         PGresult* m_res;

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


