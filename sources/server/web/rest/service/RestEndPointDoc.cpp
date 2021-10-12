#include "RestEndPointDoc.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string ContentHints::contentType() const
         {
            return m_contentType;
         }

         ContentHints& ContentHints::contentType(const std::string& s)
         {
            m_contentType = s;
            return *this;
         }

         std::string ContentHints::description() const
         {
            return m_description;
         }

         ContentHints& ContentHints::description(const std::string& s)
         {
            m_description = s;
            return *this;
         }

         const std::list<std::pair<std::string, boost::any>>& ContentHints::examples() const
         {
            return m_examples;
         }

         ContentHints& ContentHints::addExample(const std::string& title, const boost::any& example)
         {
            m_examples.emplace_back(title, example);
            return *this;
         }

         std::string RestEndPointDoc::summary() const
         {
            return m_summary;
         }

         RestEndPointDoc& RestEndPointDoc::summary(const std::string& s)
         {
            m_summary = s;
            return *this;
         }

         std::string RestEndPointDoc::description() const
         {
            return m_description;
         }

         RestEndPointDoc& RestEndPointDoc::description(const std::string& s)
         {
            m_description = s;
            return *this;
         }

         const std::list<std::string>& RestEndPointDoc::tags() const
         {
            return m_tags;
         }

         RestEndPointDoc& RestEndPointDoc::addTag(const std::string& s)
         {
            m_tags.push_back(s);
            return *this;
         }

         const std::map<shared::http::ECodes, ContentHints>& RestEndPointDoc::responses() const
         {
            return m_responses;
         }

         RestEndPointDoc& RestEndPointDoc::addResponse(const shared::http::ECodes& code,
                                                       const ContentHints& contentHints)
         {
            auto& hint = m_responses[code];
            hint.contentType(contentHints.contentType());
            hint.description(contentHints.description());
            return *this;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
