#include "stdafx.h"
#include "RestRequestHandler.h"

#include <utility>
#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>
#include <oatpp/core/data/stream/FileStream.hpp>
#include <oatpp/web/protocol/http/outgoing/BufferBody.hpp>

#include "RestRequest.h"
#include "StreamingReadCallback.h"

namespace web
{
   namespace oatppServer
   {
      CRestRequestHandler::CRestRequestHandler(std::function<boost::shared_ptr<rest::IAnswer>(boost::shared_ptr<rest::IRequest>)> handler,
                                               boost::shared_ptr<IAuthentication> authentication)
         : m_handler(std::move(handler)),
           m_authentication(std::move(authentication))
      {
      }

      std::shared_ptr<oatpp::web::server::HttpRequestHandler::OutgoingResponse> CRestRequestHandler::handle(
         const std::shared_ptr<IncomingRequest>& request)
      {
         if (m_authentication)
            m_authentication->authenticate(request);

         try
         {
            const auto answer = m_handler(boost::make_shared<CRestRequest>(request));

            std::shared_ptr<oatpp::web::protocol::http::outgoing::Body> body;
            switch (answer->bodyType())
            {
            case rest::IAnswer::EBodyType::kString:
               body = oatpp::web::protocol::http::outgoing::BufferBody::createShared(oatpp::String(answer->body()));
               break;
            case rest::IAnswer::EBodyType::kFile:
               body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
                  std::make_shared<oatpp::data::stream::FileInputStream>(answer->body().c_str()));
               break;
            case rest::IAnswer::EBodyType::kStream:
               body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
                  std::make_shared<CStreamingReadCallback>(answer->connectionHandler()));
               break;
            default: // NOLINT(clang-diagnostic-covered-switch-default)
               throw std::invalid_argument("Answer body type is invalid");
            }

            auto response = OutgoingResponse::createShared(toStatusCode(answer->code()),
                                                           body);

            static oatpp::String serverHeader(("yadoms/" + std::string(YADOMS_VERSION)).c_str());
            response->putHeader(oatpp::web::protocol::http::Header::SERVER,
                                serverHeader);

            if (answer->contentType() != rest::EContentType::kNone)
               response->putHeader(oatpp::web::protocol::http::Header::CONTENT_TYPE,
                                   ToString(answer->contentType()).c_str());

            if (answer->bodyType() == rest::IAnswer::EBodyType::kStream)
               response->putHeader("Cache-Control",
                                   "no-store");

            const auto customHeaders = answer->customHeaders();
            if (customHeaders)
               for (const auto& header : *customHeaders)
                  response->putHeader(header.first.c_str(),
                                      header.second.c_str());

            response->putHeader(oatpp::web::protocol::http::Header::CORS_ORIGIN,
                                "*");

            return response;
         }
         catch (const std::exception& exception)
         {
            YADOMS_LOG(error) << ">> Answer 500 : Internal server error : " << exception.what();
            throw HttpError(Status::CODE_500,
                            (std::string("Internal server error : ") + exception.what()).c_str());
         }
      }

      const oatpp::web::protocol::http::Status& CRestRequestHandler::toStatusCode(const shared::http::ECodes& error)
      {
         switch (error)
         {
         case shared::http::ECodes::kContinueValue: return oatpp::web::protocol::http::Status::CODE_100;
         case shared::http::ECodes::kSwitchingValue: return oatpp::web::protocol::http::Status::CODE_101;
         case shared::http::ECodes::kProcessingValue: return oatpp::web::protocol::http::Status::CODE_102;
         case shared::http::ECodes::kOKValue: return oatpp::web::protocol::http::Status::CODE_200;
         case shared::http::ECodes::kCreatedValue: return oatpp::web::protocol::http::Status::CODE_201;
         case shared::http::ECodes::kAcceptedValue: return oatpp::web::protocol::http::Status::CODE_202;
         case shared::http::ECodes::kNonAuthoritativeInformationValue: return oatpp::web::protocol::http::Status::CODE_203;
         case shared::http::ECodes::kNoContentValue: return oatpp::web::protocol::http::Status::CODE_204;
         case shared::http::ECodes::kResetContentValue: return oatpp::web::protocol::http::Status::CODE_205;
         case shared::http::ECodes::kPartialContentValue: return oatpp::web::protocol::http::Status::CODE_206;
         case shared::http::ECodes::kMultiStatusValue: return oatpp::web::protocol::http::Status::CODE_207;
         case shared::http::ECodes::kIMUsedValue: return oatpp::web::protocol::http::Status::CODE_226;
         case shared::http::ECodes::kMultipleChoicesValue: return oatpp::web::protocol::http::Status::CODE_300;
         case shared::http::ECodes::kMovedPermanentlyValue: return oatpp::web::protocol::http::Status::CODE_301;
         case shared::http::ECodes::kFoundValue: return oatpp::web::protocol::http::Status::CODE_302;
         case shared::http::ECodes::kSeeOtherValue: return oatpp::web::protocol::http::Status::CODE_303;
         case shared::http::ECodes::kNotModifiedValue: return oatpp::web::protocol::http::Status::CODE_304;
         case shared::http::ECodes::kseProxyValue: return oatpp::web::protocol::http::Status::CODE_305;
         case shared::http::ECodes::kSwitchProxyValue: return oatpp::web::protocol::http::Status::CODE_306;
         case shared::http::ECodes::kTemporaryRedirectValue: return oatpp::web::protocol::http::Status::CODE_307;
         case shared::http::ECodes::kBadRequestValue: return oatpp::web::protocol::http::Status::CODE_400;
         case shared::http::ECodes::kUnauthorizedValue: return oatpp::web::protocol::http::Status::CODE_401;
         case shared::http::ECodes::kPaymentRequiredValue: return oatpp::web::protocol::http::Status::CODE_402;
         case shared::http::ECodes::kForbiddenValue: return oatpp::web::protocol::http::Status::CODE_403;
         case shared::http::ECodes::kNotFoundValue: return oatpp::web::protocol::http::Status::CODE_404;
         case shared::http::ECodes::kMethodNotAllowedValue: return oatpp::web::protocol::http::Status::CODE_405;
         case shared::http::ECodes::kNotAcceptableValue: return oatpp::web::protocol::http::Status::CODE_406;
         case shared::http::ECodes::kProxyAuthenticationRequiredValue: return oatpp::web::protocol::http::Status::CODE_407;
         case shared::http::ECodes::kRequestTimeoutValue: return oatpp::web::protocol::http::Status::CODE_408;
         case shared::http::ECodes::kConflictValue: return oatpp::web::protocol::http::Status::CODE_409;
         case shared::http::ECodes::kGoneValue: return oatpp::web::protocol::http::Status::CODE_410;
         case shared::http::ECodes::kLengthRequiredValue: return oatpp::web::protocol::http::Status::CODE_411;
         case shared::http::ECodes::kPreconditionFailedValue: return oatpp::web::protocol::http::Status::CODE_412;
         case shared::http::ECodes::kRequestEntityTooLargeValue: return oatpp::web::protocol::http::Status::CODE_413;
         case shared::http::ECodes::kRequestURITooLongValue: return oatpp::web::protocol::http::Status::CODE_414;
         case shared::http::ECodes::kUnsupportedMediaTypeValue: return oatpp::web::protocol::http::Status::CODE_415;
         case shared::http::ECodes::kRequestedrangeunsatisfiableValue: return oatpp::web::protocol::http::Status::CODE_416;
         case shared::http::ECodes::kExpectationfailedValue: return oatpp::web::protocol::http::Status::CODE_417;
         case shared::http::ECodes::kUnprocessableentityValue: return oatpp::web::protocol::http::Status::CODE_422;
         case shared::http::ECodes::kLockedValue: return oatpp::web::protocol::http::Status::CODE_423;
         case shared::http::ECodes::kMethodfailureValue: return oatpp::web::protocol::http::Status::CODE_424;
         case shared::http::ECodes::kUnorderedCollectionValue: return oatpp::web::protocol::http::Status::CODE_425;
         case shared::http::ECodes::kUpgradeRequiredValue: return oatpp::web::protocol::http::Status::CODE_426;
         case shared::http::ECodes::kPreconditionRequiredValue: return oatpp::web::protocol::http::Status::CODE_428;
         case shared::http::ECodes::kTooManyRequestsValue: return oatpp::web::protocol::http::Status::CODE_429;
         case shared::http::ECodes::kRequestHeaderFieldsTooLargeValue: return oatpp::web::protocol::http::Status::CODE_431;
         case shared::http::ECodes::kRetryWithValue: return oatpp::web::protocol::http::Status::CODE_449;
         case shared::http::ECodes::kUnavailableForLegalReasonsValue: return oatpp::web::protocol::http::Status::CODE_451;
         case shared::http::ECodes::kNoResponseValue: return oatpp::web::protocol::http::Status::CODE_444;
         case shared::http::ECodes::kInternalServerErrorValue: return oatpp::web::protocol::http::Status::CODE_500;
         case shared::http::ECodes::kNotImplementedValue: return oatpp::web::protocol::http::Status::CODE_501;
         case shared::http::ECodes::kBadGatewayOrProxyErrorValue: return oatpp::web::protocol::http::Status::CODE_502;
         case shared::http::ECodes::kServiceUnavailableValue: return oatpp::web::protocol::http::Status::CODE_503;
         case shared::http::ECodes::kGatewayTimeoutValue: return oatpp::web::protocol::http::Status::CODE_504;
         case shared::http::ECodes::kHTTPVersionnotsupportedValue: return oatpp::web::protocol::http::Status::CODE_505;
         case shared::http::ECodes::kVariantAlsoNegotiatesValue: return oatpp::web::protocol::http::Status::CODE_506;
         case shared::http::ECodes::kInsufficientstorageValue: return oatpp::web::protocol::http::Status::CODE_507;
         case shared::http::ECodes::kLoopdetectedValue: return oatpp::web::protocol::http::Status::CODE_508;
         case shared::http::ECodes::kBandwidthLimitExceededValue: return oatpp::web::protocol::http::Status::CODE_509;
         case shared::http::ECodes::kNotextendedValue: return oatpp::web::protocol::http::Status::CODE_510;
         case shared::http::ECodes::kNetworkauthenticationrequiredValue: return oatpp::web::protocol::http::Status::CODE_511;
         default:
            YADOMS_LOG(error) << "Invalid Rest error " << static_cast<unsigned>(error);
            return Status::CODE_500;
         }
      }
   } //namespace oatppServer
} //namespace web
