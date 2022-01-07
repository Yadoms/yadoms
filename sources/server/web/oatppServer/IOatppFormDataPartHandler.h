#pragma once
#include <oatpp/web/mime/multipart/Part.hpp>
#include <oatpp/web/mime/multipart/Reader.hpp>

namespace web
{
   namespace oatppServer
   {
      //-------------------------------------
      ///\brief Form data part handler for oatpp
      //-------------------------------------
      class IOatppFormDataPartHandler
      {
      public:
         virtual ~IOatppFormDataPartHandler() = default;

         virtual std::shared_ptr<oatpp::web::mime::multipart::PartReader> partReader() = 0;
         virtual void setPart(std::shared_ptr<oatpp::web::mime::multipart::Part> part) = 0;
      };
   } // namespace oatppServer
} //namespace web
