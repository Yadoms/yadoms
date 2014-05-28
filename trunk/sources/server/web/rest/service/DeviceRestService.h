#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include <shared/event/EventHandler.hpp>
#include "communication/ISendMessageEventHandler.h"

namespace web { namespace rest { namespace service {

   class CDeviceRestService : public IRestService
   {
   public:
      CDeviceRestService(boost::shared_ptr<database::IDataProvider> dataProvider, communication::ISendMessageAsync & messageSender);
      virtual ~CDeviceRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();

   private:
      //-----------------------------------------
      ///\brief   get a device
      //-----------------------------------------
      web::rest::json::CJson getOneDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      //-----------------------------------------
      ///\brief   get all devices
      //-----------------------------------------
      web::rest::json::CJson getAllDevices(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      //-----------------------------------------
      ///\brief   get the last data for each keyword supported by a device
      //-----------------------------------------
      web::rest::json::CJson getLastDeviceData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      //-----------------------------------------
      ///\brief   send a command to a device
      //-----------------------------------------
      web::rest::json::CJson sendDeviceCommand(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      //-----------------------------------------
      ///\brief   create a device in database
      //-----------------------------------------
      web::rest::json::CJson createDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      //-----------------------------------------
      ///\brief   get a device data
      //-----------------------------------------
      web::rest::json::CJson getDeviceData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      //-----------------------------------------
      ///\brief   delete a device in database
      //-----------------------------------------
      web::rest::json::CJson deleteDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

   private:
      //-----------------------------------------
      ///\brief   Transactional method used to encapsulate operation in transactions
      //-----------------------------------------
      web::rest::json::CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

      //-----------------------------------------
      ///\brief   Data provider
      //-----------------------------------------
      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      //-----------------------------------------
      ///\brief   The rest keyword which identifies this rule
      //-----------------------------------------
      static std::string m_restKeyword;

      //-----------------------------------------
      ///\brief   The send message interface
      //-----------------------------------------
      communication::ISendMessageAsync & m_messageSender;
   };


} //namespace service
} //namespace rest
} //namespace web 
