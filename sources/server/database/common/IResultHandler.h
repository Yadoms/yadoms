#pragma once

#include <shared/DataContainer.h>
#include <Poco/DateTime.h>

namespace database
{
   namespace common
   {
      class IResultHandler
      {
      public:
         virtual ~IResultHandler() = default;

         virtual int getColumnCount() = 0;
         virtual std::string getColumnName(int columnIndex) = 0;
         virtual bool next_step() = 0;
         virtual std::string extractValueAsString(int columnIndex) = 0;
         virtual int extractValueAsInt(int columnIndex) = 0;
         virtual float extractValueAsFloat(int columnIndex) = 0;
         virtual double extractValueAsDouble(int columnIndex) = 0;
         virtual unsigned char* extractValueAsBlob(int columnIndex) = 0;
         virtual bool extractValueAsBool(int columnIndex) = 0;
         virtual bool isValueNull(int columnIndex) = 0;
         virtual boost::posix_time::ptime extractValueAsBoostTime(int columnIndex) = 0;
         virtual Poco::DateTime extractValueAsPocoTime(int columnIndex) = 0;
         virtual boost::shared_ptr<shared::CDataContainer> extractValueAsDataContainer(int columnIndex) = 0;
      };
   } //namespace common
} //namespace database 
