#pragma once

#include "CoreMinimal.h"
#include "SimpleHttpServerResponseCodes.generated.h"

UENUM(BlueprintType)
enum class ESimpleHttpServerResponseCodes : uint8
{
	// status code not set yet
	Unknown,

	// --- 1xx (Hidden) ---
	// the request can be continued.
	Continue UMETA(Hidden),
	// the server has switched protocols in an upgrade header.
	SwitchProtocol UMETA(Hidden),

	// --- 2xx ---
	// the request completed successfully.
	Ok,
	// the request has been fulfilled and resulted in the creation of a new resource.
	Created UMETA(Hidden),
	// the request has been accepted for processing, but the processing has not been completed.
	Accepted UMETA(Hidden),
	// the returned meta information in the entity-header is not the definitive set available from the origin server.
	Partial UMETA(Hidden),
	// the server has fulfilled the request, but there is no new information to send back.
	NoContent UMETA(Hidden),
	// the request has been completed, and the client program should reset the document view that caused the request to be sent to allow the user to easily initiate another input action.
	ResetContent UMETA(Hidden),
	// the server has fulfilled the partial get request for the resource.
	PartialContent UMETA(Hidden),

	// --- 3xx (Hidden) ---
	// the server couldn't decide what to return.
	Ambiguous UMETA(Hidden),
	// the requested resource has been assigned to a new permanent uri (uniform resource identifier), and any future references to this resource should be done using one of the returned uris.
	Moved UMETA(Hidden),
	// the requested resource resides temporarily under a different uri (uniform resource identifier).
	Redirect UMETA(Hidden),
	// the response to the request can be found under a different uri (uniform resource identifier) and should be retrieved using a get http verb on that resource.
	RedirectMethod UMETA(Hidden),
	// the requested resource has not been modified.
	NotModified UMETA(Hidden),
	// the requested resource must be accessed through the proxy...
	UseProxy UMETA(Hidden),
	// the redirected request keeps the same http verb. http/1.1 behavior.
	RedirectKeepVerb UMETA(Hidden),

	// --- 4xx ---
	// the request could not be processed by the server due to invalid syntax.
	BadRequest,
	// the requested resource requires user authentication.
	Denied,
	// not currently implemented in the http protocol.
	PaymentReq UMETA(Hidden),
	// the server understood the request, but is refusing to fulfill it.
	Forbidden,
	// the server has not found anything matching the requested uri (uniform resource identifier).
	NotFound,
	// the http verb used is not allowed.
	BadMethod,
	// no responses acceptable to the client were found.
	NoneAcceptable UMETA(Hidden),
	// proxy authentication required.
	ProxyAuthReq UMETA(Hidden),
	// the server timed out waiting for the request.
	RequestTimeout,
	// the request could not be completed due to a conflict with the current state of the resource. the user should resubmit with more information.
	Conflict UMETA(Hidden),
	// the requested resource is no longer available at the server, and no forwarding address is known.
	Gone UMETA(Hidden),
	// the server refuses to accept the request without a defined content length.
	LengthRequired UMETA(Hidden),
	// the precondition given in one or more of the request header fields evaluated to false when it was tested on the server.
	PrecondFailed UMETA(Hidden),
	// the server is refusing to process a request because the request entity is larger than the server is willing or able to process.
	RequestTooLarge UMETA(Hidden),
	// the server is refusing to service the request because the request uri (uniform resource identifier) is longer than the server is willing to interpret.
	UriTooLong UMETA(Hidden),
	// the server is refusing to service the request because the entity of the request is in a format not supported...
	UnsupportedMedia,
	// too many requests, the server is throttling
	TooManyRequests,
	// the request should be retried after doing the appropriate action.
	RetryWith,

	// --- 5xx ---
	// the server encountered an unexpected condition that prevented it from fulfilling the request.
	ServerError,
	// the server does not support the functionality required...
	NotSupported,
	// the server does not support the functionality required to fulfill the request.
	BadGateway,
	// the service is temporarily overloaded.
	ServiceUnavail UMETA(Hidden),
	// the request was timed out waiting for a gateway.
	GatewayTimeout,
	// the server does not support, or refuses to support, the http protocol version that was used in the request message.
	VersionNotSup UMETA(Hidden),
};


static EHttpServerResponseCodes ToHttpServerResponseCode(ESimpleHttpServerResponseCodes In)
{
	switch (In)
	{
	case ESimpleHttpServerResponseCodes::Unknown:
		return EHttpServerResponseCodes::Unknown;

	case ESimpleHttpServerResponseCodes::Continue:
		return EHttpServerResponseCodes::Continue;

	case ESimpleHttpServerResponseCodes::SwitchProtocol:
		return EHttpServerResponseCodes::SwitchProtocol;

	case ESimpleHttpServerResponseCodes::Ok:
		return EHttpServerResponseCodes::Ok;

	case ESimpleHttpServerResponseCodes::Created:
		return EHttpServerResponseCodes::Created;

	case ESimpleHttpServerResponseCodes::Accepted:
		return EHttpServerResponseCodes::Accepted;

	case ESimpleHttpServerResponseCodes::Partial:
		return EHttpServerResponseCodes::Partial;

	case ESimpleHttpServerResponseCodes::NoContent:
		return EHttpServerResponseCodes::NoContent;

	case ESimpleHttpServerResponseCodes::ResetContent:
		return EHttpServerResponseCodes::ResetContent;

	case ESimpleHttpServerResponseCodes::PartialContent:
		return EHttpServerResponseCodes::PartialContent;

	case ESimpleHttpServerResponseCodes::Ambiguous:
		return EHttpServerResponseCodes::Ambiguous;

	case ESimpleHttpServerResponseCodes::Moved:
		return EHttpServerResponseCodes::Moved;

	case ESimpleHttpServerResponseCodes::Redirect:
		return EHttpServerResponseCodes::Redirect;

	case ESimpleHttpServerResponseCodes::RedirectMethod:
		return EHttpServerResponseCodes::RedirectMethod;

	case ESimpleHttpServerResponseCodes::NotModified:
		return EHttpServerResponseCodes::NotModified;

	case ESimpleHttpServerResponseCodes::UseProxy:
		return EHttpServerResponseCodes::UseProxy;

	case ESimpleHttpServerResponseCodes::RedirectKeepVerb:
		return EHttpServerResponseCodes::RedirectKeepVerb;

	case ESimpleHttpServerResponseCodes::BadRequest:
		return EHttpServerResponseCodes::BadRequest;

	case ESimpleHttpServerResponseCodes::Denied:
		return EHttpServerResponseCodes::Denied;

	case ESimpleHttpServerResponseCodes::PaymentReq:
		return EHttpServerResponseCodes::PaymentReq;

	case ESimpleHttpServerResponseCodes::Forbidden:
		return EHttpServerResponseCodes::Forbidden;

	case ESimpleHttpServerResponseCodes::NotFound:
		return EHttpServerResponseCodes::NotFound;

	case ESimpleHttpServerResponseCodes::BadMethod:
		return EHttpServerResponseCodes::BadMethod;

	case ESimpleHttpServerResponseCodes::NoneAcceptable:
		return EHttpServerResponseCodes::NoneAcceptable;

	case ESimpleHttpServerResponseCodes::ProxyAuthReq:
		return EHttpServerResponseCodes::ProxyAuthReq;

	case ESimpleHttpServerResponseCodes::RequestTimeout:
		return EHttpServerResponseCodes::RequestTimeout;

	case ESimpleHttpServerResponseCodes::Conflict:
		return EHttpServerResponseCodes::Conflict;

	case ESimpleHttpServerResponseCodes::Gone:
		return EHttpServerResponseCodes::Gone;

	case ESimpleHttpServerResponseCodes::LengthRequired:
		return EHttpServerResponseCodes::LengthRequired;

	case ESimpleHttpServerResponseCodes::PrecondFailed:
		return EHttpServerResponseCodes::PrecondFailed;

	case ESimpleHttpServerResponseCodes::RequestTooLarge:
		return EHttpServerResponseCodes::RequestTooLarge;

	case ESimpleHttpServerResponseCodes::UriTooLong:
		return EHttpServerResponseCodes::UriTooLong;

	case ESimpleHttpServerResponseCodes::UnsupportedMedia:
		return EHttpServerResponseCodes::UnsupportedMedia;

	case ESimpleHttpServerResponseCodes::TooManyRequests:
		return EHttpServerResponseCodes::TooManyRequests;

	case ESimpleHttpServerResponseCodes::RetryWith:
		return EHttpServerResponseCodes::RetryWith;

	case ESimpleHttpServerResponseCodes::ServerError:
		return EHttpServerResponseCodes::ServerError;

	case ESimpleHttpServerResponseCodes::NotSupported:
		return EHttpServerResponseCodes::NotSupported;

	case ESimpleHttpServerResponseCodes::BadGateway:
		return EHttpServerResponseCodes::BadGateway;

	case ESimpleHttpServerResponseCodes::ServiceUnavail:
		return EHttpServerResponseCodes::ServiceUnavail;

	case ESimpleHttpServerResponseCodes::GatewayTimeout:
		return EHttpServerResponseCodes::GatewayTimeout;

	case ESimpleHttpServerResponseCodes::VersionNotSup:
		return EHttpServerResponseCodes::VersionNotSup;

	default:
		return EHttpServerResponseCodes::ServerError;
	}
}
