#pragma once

#include <shared/DataContainer.h>
#include <shared/IDataContainable.h>

namespace web { namespace rest { 


   //-----------------------------------------
   ///\brief   Class which generate REST result messages
   //-----------------------------------------
   class CResult
   {
   public:
      //-----------------------------------------
      ///\brief   Destructor
      //-----------------------------------------
      virtual ~CResult() {}

      //-----------------------------------------
      ///\brief   Generate an error JSON message
      ///\param [in] errorMessage : the error message
      ///\param [in] data : optionnal additionnal data
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      static boost::shared_ptr<shared::CDataContainer> GenerateError(const std::string & errorMessage = std::string(), const shared::CDataContainer & data = shared::CDataContainer::EmptyContainer);

      //-----------------------------------------
      ///\brief   Generate an error JSON message
      ///\param [in] error : the exception
      ///\param [in] data : optionnal additionnal data
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      static boost::shared_ptr<shared::CDataContainer> GenerateError(const std::exception & error, const shared::CDataContainer & data = shared::CDataContainer::EmptyContainer);

      //-----------------------------------------
      ///\brief   Generate a success JSON message
      ///\param [in] data : optionnal additionnal data
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      static boost::shared_ptr<shared::CDataContainer> GenerateSuccess(const shared::CDataContainer & data = shared::CDataContainer::EmptyContainer);
      
      //-----------------------------------------
      ///\brief   Generate a success JSON message
      ///\param [in] stringData : data as string
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      static boost::shared_ptr<shared::CDataContainer> GenerateSuccess(const std::string & stringData);

      //-----------------------------------------
      ///\brief   Generate a success JSON message
      ///\param [in] data : a datacontainable object
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      template<class T>
      static boost::shared_ptr<shared::CDataContainer> GenerateSuccess(boost::shared_ptr<T> dataContainableObject)
      {
         return GenerateInternal(true, std::string(), dataContainableObject);
      }

      //-----------------------------------------
      ///\brief   check if the shared::CDataContainer data is a success messsage
      ///\return  true if the shared::CDataContainer data is a success messsage
      //-----------------------------------------
      static bool isSuccess(const shared::CDataContainer & data);

   private:
      //-----------------------------------------
      ///\brief   Generate a JSON message
      ///\param [in] result : true for success, false for warning
      ///\param [in] errorMessage : error message (can be empty)
      ///\param [in] data : additionnal data (can be empty)
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      static boost::shared_ptr<shared::CDataContainer> GenerateInternal(const bool result, const std::string & message, const shared::CDataContainer & data);
      
      //-----------------------------------------
      ///\brief   Generate a JSON message
      ///\param [in] result : true for success, false for warning
      ///\param [in] errorMessage : error message (can be empty)
      ///\param [in] data : string
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      static boost::shared_ptr<shared::CDataContainer> GenerateInternal(const bool result, const std::string & message, const std::string & data);

      //-----------------------------------------
      ///\brief   Generate a JSON message
      ///\param [in] result : true for success, false for warning
      ///\param [in] errorMessage : error message (can be empty)
      ///\param [in] data : a datacontainable object
      ///\return  the message as shared::CDataContainer format
      //-----------------------------------------
      template<class T>
      static boost::shared_ptr<shared::CDataContainer> GenerateInternal(const bool result, const std::string & message, boost::shared_ptr<T> data)
      {
         boost::shared_ptr<shared::CDataContainer> error = boost::make_shared<shared::CDataContainer>();
         error->set(m_resultFieldName, result);
         error->set(m_errorMessageFieldName, message);
         error->set(m_dataFieldName, data);
         return error;
      }

   private:
      //-----------------------------------------
      ///\brief   Identifier for result in Json
      //-----------------------------------------
      static std::string m_resultFieldName;
      //-----------------------------------------
      ///\brief   Identifier for error message in Json
      //-----------------------------------------
      static std::string m_errorMessageFieldName;
      //-----------------------------------------
      ///\brief   Identifier for additionnal data in Json
      //-----------------------------------------
      static std::string m_dataFieldName;
   };



} //namespace rest
} //namespace web 

