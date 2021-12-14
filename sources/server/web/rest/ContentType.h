#pragma once

namespace web
{
   namespace rest
   {
      /// @brief HTTP REST content type
      enum class EContentType
      {
         kFormUrlEncoded,
         kOctetStream,
         kPlainText,
         kJson,
         kNone
      };

      static const std::string ContentTypeFormUrlEncoded("application/x-www-form-urlencoded");
      static const std::string ContentTypeOctetStreamString("application/octet-stream");
      static const std::string ContentTypePlainTextString("text/plain");
      static const std::string ContentTypeJsonString("application/json");

      static const std::string& ToString(const EContentType& contentType)
      {
         switch (contentType)
         {
         case EContentType::kFormUrlEncoded: return ContentTypeFormUrlEncoded;
         case EContentType::kOctetStream: return ContentTypeOctetStreamString;
         case EContentType::kPlainText: return ContentTypePlainTextString;
         case EContentType::kJson: return ContentTypeJsonString;
         default: // NOLINT(clang-diagnostic-covered-switch-default)
            throw std::out_of_range(
               "Can not convert HTTP REST content type into string, invalid content type : " + std::to_string(
                  static_cast<unsigned int>(contentType)));
         }
      }

      static EContentType ToContentType(const std::string& contentType)
      {
         if (contentType == ContentTypeFormUrlEncoded)
            return EContentType::kFormUrlEncoded;
         if (contentType == ContentTypeOctetStreamString)
            return EContentType::kOctetStream;
         if (contentType == ContentTypePlainTextString)
            return EContentType::kPlainText;
         if (contentType == ContentTypeJsonString)
            return EContentType::kJson;

         throw std::out_of_range("Can not convert HTTP REST content type from string, invalid content type : " + contentType);
      }
   }
} // namespace web::rest
