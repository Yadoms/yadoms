#include "stdafx.h"
#include "RestAccessPoint.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CRestAccessPoint::CRestAccessPoint(const shared::http::ERestMethod& method,
                                            const std::string& path,
                                            std::function<boost::shared_ptr<IRestAnswer>(std::map<std::string, std::string>, std::string)> handler)
            : m_method(method),
              m_path(path),
              m_handler(std::move(handler))
         {
         }

         shared::http::ERestMethod CRestAccessPoint::method() const
         {
            return m_method;
         }

         std::string CRestAccessPoint::path() const
         {
            return m_path;
         }

         std::function<boost::shared_ptr<IRestAnswer>(std::map<std::string, std::string>, std::string)> CRestAccessPoint::handler() const
         {
            return m_handler;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
