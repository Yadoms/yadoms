#include "stdafx.h"
#include "NoContentAnswer.h"

namespace web
{
   namespace rest
   {
      shared::http::ECodes CNoContentAnswer::code() const
      {
         return shared::http::ECodes::kNoContent;
      }

      std::string CNoContentAnswer::body() const
      {
         return std::string();
      }

      EContentType CNoContentAnswer::contentType() const
      {
         return EContentType::kNone;
      }
   } //namespace rest
} //namespace web 
