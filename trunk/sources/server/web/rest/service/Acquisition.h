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
      shared::CDataContainer getAcquisitionById(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   


      //-----------------------------------------
      ///\brief   Get the last data of a keyword
      //-----------------------------------------
      shared::CDataContainer getKeywordLastData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   

      //-----------------------------------------
      ///\brief   Get the data of a keyword (with optional date filter)
      //-----------------------------------------
      shared::CDataContainer getKeywordData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      
      //-----------------------------------------
      ///\brief   Get the data of a keyword (with optional date filter) for highchart treatment (fast queries, optimized for highcharts js)
      //-----------------------------------------
      shared::CDataContainer getHighchartKeywordData(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);


      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
