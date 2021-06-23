#pragma once
#include <shared/DataContainer.h>
#include <shared/http/Codes.h>

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
      };

      class CSuccessRestAnswer final : public IRestAnswer //TODO ménage
      {
      public:
         CSuccessRestAnswer(const shared::CDataContainer& result)
            : m_code(shared::http::ECodes::kOK),
              m_body(result.serialize())
         {
         }

         CSuccessRestAnswer(const std::string& result)
            : m_code(shared::http::ECodes::kOK),
              m_body(result)
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

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
      };

      class CErrorRestAnswer final : public IRestAnswer //TODO ménage
      {
      public:
         CErrorRestAnswer(const shared::http::ECodes& code,
                          const std::string& message)
            : m_code(code),
              m_body(message)
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

      private:
         const shared::http::ECodes m_code;
         const std::string m_body;
      };
   } //namespace rest
} //namespace web 
