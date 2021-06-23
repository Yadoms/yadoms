#pragma once
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include "IRestAccessPoint.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CRestAccessPoint final : public IRestAccessPoint //TODO déplacer dans namespace oatpp
         {
         public:
            CRestAccessPoint(const shared::http::ERestMethod& method,
                             const std::string& path,
                             std::function<boost::shared_ptr<IRestAnswer>(std::map<std::string, std::string>, std::string)> handler);
            ~CRestAccessPoint() override = default;

            // IRestAccessPoint implementation
            shared::http::ERestMethod method() const override;
            std::string path() const override;
            std::function<boost::shared_ptr<IRestAnswer>(std::map<std::string, std::string>, std::string)> handler() const override;
            // [END] IRestAccessPoint implementation

         private:
            const shared::http::ERestMethod m_method;
            const std::string m_path;
            std::function<boost::shared_ptr<IRestAnswer>(std::map<std::string, std::string>, std::string)> m_handler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
