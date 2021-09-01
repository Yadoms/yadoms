#pragma once

namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	REST Method type
      //--------------------------------------------------------------
      enum class ERestMethod
      {
         kGet = 0,
         kPut,
         kPost,
         kHead
      };

      static const std::string RestMethodGetString("GET");
      static const std::string RestMethodPutString("PUT");
      static const std::string RestMethodPostString("POST");
      static const std::string RestMethodHeadString("HEAD");

      static const std::string& ToString(const ERestMethod& method)
      {
         switch (method)
         {
         case ERestMethod::kGet: return RestMethodGetString;
         case ERestMethod::kPut: return RestMethodPutString;
         case ERestMethod::kPost: return RestMethodPostString;
         case ERestMethod::kHead: return RestMethodHeadString;
         default:  // NOLINT(clang-diagnostic-covered-switch-default)
            throw std::out_of_range(
               "Can not convert HTTP REST method into string, invalid method : " + std::to_string(static_cast<unsigned int>(method)));
         }
      }

      static ERestMethod ToRestMethod(const std::string& method)
      {
         if (method == RestMethodGetString)
            return ERestMethod::kGet;
         if (method == RestMethodPutString)
            return ERestMethod::kPut;
         if (method == RestMethodPostString)
            return ERestMethod::kPost;
         if (method == RestMethodHeadString)
            return ERestMethod::kHead;

         throw std::out_of_range("Can not convert HTTP REST method from string, invalid method : " + method);
      }
   }
} // namespace shared::http
