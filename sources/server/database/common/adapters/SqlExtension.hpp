#pragma once
#include "database/common/IResultHandler.h"
#include <shared/StringExtension.h>

namespace database { 
namespace common { 
namespace adapters { 


   //--------------------------------------------------------------
   ///\brief   Class (static) used to extend sqlite3 functions.
   ///         It provides a method to handle utf8 in std::string
   ///         and a templated function which can extract typed data from statement
   //--------------------------------------------------------------
   class CSqlExtension
   {
   public:
      //--------------------------------------------------------------
      ///\brief  Destructor
      //--------------------------------------------------------------
      virtual ~CSqlExtension() {}

      //--------------------------------------------------------------
      ///\brief  Extract a column value in a typed way
      ///\param [in]    pStmt    The current statement
      ///\param [in]    i        The index (zero based) of the targeted column
      ///\template      TValue   The type of the expectted value
      ///\return The value from the column.
      //--------------------------------------------------------------
      template<class TValue>
      static inline TValue extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol);
   };

   //--------------------------------------------------------------
   ///\brief  Default method. Extract data as text and use CStringExtension to convert it in good type
   //--------------------------------------------------------------
   template<class TValue>
   inline TValue CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      //as default way, read text from database and extractData it with c++ side functions
      return shared::CStringExtension::parse<TValue>(resultHandler->extractValueAsString(nCol));
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = int (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline int CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsInt(nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = float (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline float CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsFloat(nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = int (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline double CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsDouble(nCol); 
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = unsigned char * (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline unsigned char* CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsBlob(nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = std::string (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline std::string CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsString(nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = std::string (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline boost::tuples::null_type CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return boost::tuples::null_type();
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = bool (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline bool CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsBool(nCol); 
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = boost::posix_time::ptime (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline boost::posix_time::ptime CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsBoostTime(nCol);
   }  
   
   //--------------------------------------------------------------
   ///\brief  Override method for type = Poco::DateTime (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline Poco::DateTime CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsPocoTime(nCol);
   }

   //--------------------------------------------------------------
   ///\brief  Override method for type = shared::CDataContainer (use specific sqlite3 function)
   //--------------------------------------------------------------
   template<>
   inline shared::CDataContainerSharedPtr CSqlExtension::extractData(boost::shared_ptr<IResultHandler> resultHandler, int nCol)
   {
      return resultHandler->extractValueAsDataContainer(nCol);
   }




} //namespace adapters
} //namespace common
} //namespace database 

   