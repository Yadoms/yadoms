#pragma once

#include <shared/DataContainer.h>
#include <Poco/DateTime.h>

namespace database { 
namespace common { 

   class IResultHandler 
   {
   public:
      virtual ~IResultHandler() {}

      virtual int getColumnCount() = 0;
      virtual std::string getColumnName(const int columnIndex) = 0;
      virtual bool next_step() = 0;
      virtual std::string extractValueAsString(const int columnIndex) = 0;
      virtual int extractValueAsInt(const int columnIndex) = 0;
      virtual float extractValueAsFloat(const int columnIndex) = 0;
      virtual double extractValueAsDouble(const int columnIndex) = 0;
      virtual unsigned char* extractValueAsBlob(const int columnIndex) = 0;
      virtual bool extractValueAsBool(const int columnIndex) = 0;
      virtual bool isValueNull(const int columnIndex) = 0;
      virtual boost::posix_time::ptime extractValueAsBoostTime(const int columnIndex) = 0;
      virtual Poco::DateTime extractValueAsPocoTime(const int columnIndex) = 0;
      virtual boost::shared_ptr<shared::CDataContainer> extractValueAsDataContainer(const int columnIndex) = 0;
   };


} //namespace common
} //namespace database 

