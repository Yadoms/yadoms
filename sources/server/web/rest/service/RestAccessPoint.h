#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include "IRestAccessPoint.h"
#include "web/rest/RequestV2.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CRestAccessPoint final : public IRestAccessPoint //TODO déplacer dans namespace oatpp (ou pas ? ? ?)
         {
         public:
            CRestAccessPoint(const shared::http::ERestMethod& method,
                             const std::string& path,
                             std::function<boost::shared_ptr<IRestAnswer>(boost::shared_ptr<IRestRequest>)> handler);
            ~CRestAccessPoint() override = default;

            // IRestAccessPoint implementation
            shared::http::ERestMethod method() const override;
            std::string path() const override;
            std::function<boost::shared_ptr<IRestAnswer>(boost::shared_ptr<IRestRequest>)> handler() const override;
            // [END] IRestAccessPoint implementation

         private:
            const shared::http::ERestMethod m_method;
            const std::string m_path;
            std::function<boost::shared_ptr<IRestAnswer>(boost::shared_ptr<IRestRequest>)> m_handler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
