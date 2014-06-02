#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include <shared/Log.h>

namespace web { namespace rest { namespace service {

   std::string CAcquisition::m_restKeyword= std::string("acquisition");


   CAcquisition::CAcquisition(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {

   }


   CAcquisition::~CAcquisition()
   {
   }


   void CAcquisition::configureDispatcher(CRestDispatcher & dispatcher)
   {
   }

   const std::string & CAcquisition::getRestKeyword()
   {
      return m_restKeyword;
   }




} //namespace service
} //namespace rest
} //namespace web 
