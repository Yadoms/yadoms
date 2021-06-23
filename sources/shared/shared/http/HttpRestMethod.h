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

      static const std::string& ToString(const ERestMethod& method)
      {
         static const std::string GetString("GET");
         static const std::string PutString("PUT");
         static const std::string PostString("POST");
         static const std::string HeadString("HEAD");

         switch (method)
         {
         case ERestMethod::kGet: return GetString;
         case ERestMethod::kPut: return PutString;
         case ERestMethod::kPost: return PostString;
         case ERestMethod::kHead: return HeadString;
         default:
            throw std::out_of_range(
               "Can not convert HTTP REST method into string, invalid method : " + std::to_string(static_cast<unsigned int>(method)));
         }
      }
   }
} // namespace shared::http
