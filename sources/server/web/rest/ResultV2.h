#pragma once
#include <utility>
#include <shared/DataContainer.h>
#include <shared/http/Codes.h>
#include "ContentType.h"

//TODO renommer le fichier

namespace web
{
   namespace rest
   {
      class IRestAnswer //TODO ménage
      {
      public:
         virtual ~IRestAnswer() = default;

         virtual shared::http::ECodes code() const = 0;
         virtual std::string body() const = 0;
         virtual EContentType contentType() const = 0;
      };

      class CSuccessRestAnswer final : public IRestAnswer //TODO ménage
      {
      public:
         CSuccessRestAnswer(const shared::CDataContainer& result)
            : m_code(shared::http::ECodes::kOK),
              m_body(result.serialize()),
              m_contentType(EContentType::kJson)
         {
         }

         CSuccessRestAnswer(std::string result,
                            const EContentType& contentType = EContentType::kPlainText)
            : m_code(shared::http::ECodes::kOK),
              m_body(std::move(result)),
              m_contentType(contentType)
         {
         }

         ~CSuccessRestAnswer() override = default;

         shared::http::ECodes code() const override
         {
            return m_code;
         }

         std::string body() const override
         {
            return m_body;
         }

         EContentType contentType() const override
         {
            return m_contentType;
         }

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
         const EContentType m_contentType;
      };

      class CErrorRestAnswer final : public IRestAnswer //TODO ménage
      {
      public:
         CErrorRestAnswer(const shared::http::ECodes& code,
                          std::string message)
            : m_code(code),
              m_body(std::move(message))
         {
         }

         ~CErrorRestAnswer() override = default;

         shared::http::ECodes code() const override
         {
            return m_code;
         }

         std::string body() const override
         {
            return m_body;
         }

         EContentType contentType() const override
         {
            return EContentType::kPlainText;
         }

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
      };
   } //namespace rest
} //namespace web 
