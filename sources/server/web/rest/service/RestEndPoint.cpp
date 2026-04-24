#include "stdafx.h"
#include "RestEndPoint.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CRestEndPoint::CRestEndPoint(const shared::http::ERestVerb& method,
                                      const std::string& path,
                                      std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)> handler)
            : m_method(method),
              m_path(path),
              m_handler(std::move(handler))
         {
         }

         shared::http::ERestVerb CRestEndPoint::verb() const
         {
            return m_method;
         }

         std::string CRestEndPoint::path() const
         {
            return m_path;
         }

         std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)> CRestEndPoint::handler() const
         {
            return m_handler;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
