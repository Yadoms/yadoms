#pragma once
#include <list>
#include <string>
#include <unordered_map>

#include <shared/http/Codes.h>


namespace web
{
   namespace rest
   {
      namespace service
      {

         //----------------------------
         ///\brief   REST end-point response documentation
         //----------------------------
         class ContentHints //TODO déplacer
         {
         public:
            virtual ~ContentHints() = default;

            std::string contentType() const;
            ContentHints& contentType(const std::string& s);

            std::string description() const;
            ContentHints& description(const std::string& s);

            const std::list<std::pair<std::string, boost::any>>& examples() const;
            ContentHints& addExample(const std::string& title, const boost::any& example);

         private:
            std::string m_contentType;
            std::string m_description;
            std::list<std::pair<std::string, boost::any>> m_examples;
         };

         //----------------------------
         ///\brief   REST end-point documentation
         //----------------------------
         class RestEndPointDoc
         {
         public:
            virtual ~RestEndPointDoc() = default;

            std::string summary() const;
            RestEndPointDoc& summary(const std::string& s);

            std::string description() const;
            RestEndPointDoc& description(const std::string& s);

            const std::list<std::string>& tags() const;
            RestEndPointDoc& addTag(const std::string& s);

            const std::map<shared::http::ECodes, ContentHints>& responses() const;
            RestEndPointDoc& addResponse(const shared::http::ECodes& code, const ContentHints& contentHints);

         private:
            std::string m_summary;
            std::list<std::string> m_tags;
            std::string m_description;
            std::map<shared::http::ECodes, ContentHints> m_responses;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
