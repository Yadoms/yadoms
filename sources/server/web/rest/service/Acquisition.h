#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CAcquisition : public IRestService
   {
   public:
      CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CAcquisition();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();

   private:
      //-----------------------------------------
      ///\brief   Get an acquisition from the id
      //-----------------------------------------
      web::rest::json::CJson getAcquisitionById(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   


      //-----------------------------------------
      ///\brief   Get the last data of a keyword
      //-----------------------------------------
      web::rest::json::CJson getKeywordLastData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      //-----------------------------------------
      ///\brief   Get the data of a keyword (with optional date filter)
      //-----------------------------------------
      web::rest::json::CJson getKeywordData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   


      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
