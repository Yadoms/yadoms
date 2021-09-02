#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include "IRestEndPoint.h"
#include "web/rest/IRequest.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CRestEndPoint final : public IRestEndPoint
         {
         public:
            CRestEndPoint(const shared::http::ERestVerb& method,
                          const std::string& path,
                          std::function<boost::shared_ptr<IRestAnswer>(boost::shared_ptr<IRequest>)> handler);
            ~CRestEndPoint() override = default;

            // IRestEndPoint implementation
            shared::http::ERestVerb verb() const override;
            std::string path() const override;
            std::function<boost::shared_ptr<IRestAnswer>(boost::shared_ptr<IRequest>)> handler() const override;
            // [END] IRestEndPoint implementation

         private:
            const shared::http::ERestVerb m_method;
            const std::string m_path;
            std::function<boost::shared_ptr<IRestAnswer>(boost::shared_ptr<IRequest>)> m_handler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
