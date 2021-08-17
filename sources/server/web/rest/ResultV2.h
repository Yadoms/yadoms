#pragma once
#include <shared/DataContainer.h>
#include <shared/http/Codes.h>

#include <utility>

//TODO renommer le fichier

namespace web
{
   namespace rest
   {
      enum class BodyType
      {
         kPlainText,
         kJson
      };

      class IRestAnswer //TODO ménage
      {
      public:
         virtual ~IRestAnswer() = default;

         virtual shared::http::ECodes code() const = 0;
         virtual std::string body() const = 0;
         virtual BodyType bodyType() const = 0;
      };

      class CSuccessRestAnswer final : public IRestAnswer //TODO ménage
      {
      public:
         CSuccessRestAnswer(const shared::CDataContainer& result)
            : m_code(shared::http::ECodes::kOK),
              m_body(result.serialize()),
              m_bodyType(BodyType::kJson)
         {
         }

         CSuccessRestAnswer(std::string result,
                            const BodyType& bodyType = BodyType::kPlainText)
            : m_code(shared::http::ECodes::kOK),
              m_body(std::move(result)),
              m_bodyType(bodyType)
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

         BodyType bodyType() const override
         {
            return m_bodyType;
         }

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
         const BodyType m_bodyType;
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

         BodyType bodyType() const override
         {
            return BodyType::kPlainText;
         }

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
      };
   } //namespace rest
} //namespace web 
