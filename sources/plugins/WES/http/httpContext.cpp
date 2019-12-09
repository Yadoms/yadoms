#include "stdafx.h"
#include "httpContext.h"

namespace http
{
	httpContext::httpContext():
		m_credentialNeeded(false)
	{}

	httpContext::~httpContext()
	{}

	bool httpContext::basicCredential() {
		return m_credentialNeeded;
	}

	void httpContext::setBasicCredential(bool value) {
		m_credentialNeeded = value;
	}
} // namespace http