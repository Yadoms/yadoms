#pragma once
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
                          std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)> handler);
            ~CRestEndPoint() override = default;

            // IRestEndPoint implementation
            shared::http::ERestVerb verb() const override;
            std::string path() const override;
            boost::shared_ptr<RestEndPointDoc> doc() const override;
            std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)> handler() const override;
            // [END] IRestEndPoint implementation

         private:
            const shared::http::ERestVerb m_method;
            const std::string m_path;
            std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)> m_handler;
            boost::shared_ptr<RestEndPointDoc> m_doc;
         };

         /// Macro to simplify adding a new endpoint
#define MAKE_ENDPOINT(verb, path, handler) \
          boost::make_shared<CRestEndPoint>(shared::http::ERestVerb::verb, \
          path, \
          [this](boost::shared_ptr<IRequest> request) \
               { return handler(request); })
      } //namespace service
   } //namespace rest
} //namespace web 
