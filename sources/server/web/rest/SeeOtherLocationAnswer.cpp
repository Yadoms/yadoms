#include "stdafx.h"
#include "SeeOtherLocationAnswer.h"

using namespace web::rest;

CSeeOtherLocationAnswer::CSeeOtherLocationAnswer(const std::string& resourceCreatedPath)
   : m_customHeaders(std::make_shared<std::map<std::string, std::string>>())
{
   (*m_customHeaders)["Location"] = resourceCreatedPath;
}

shared::http::ECodes CSeeOtherLocationAnswer::code() const
{
   return shared::http::ECodes::kSeeOther;
}

std::string CSeeOtherLocationAnswer::body() const
{
   return {};
}

bool CSeeOtherLocationAnswer::bodyIsFile() const
{
   return false;
}

EContentType CSeeOtherLocationAnswer::contentType() const
{
   return EContentType::kNone;
}

std::shared_ptr<std::map<std::string, std::string>> CSeeOtherLocationAnswer::customHeaders() const
{
   return m_customHeaders;
}
