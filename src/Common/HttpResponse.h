#pragma once

#include "HttpHeader.h"

class HttpResponse
{
public:
	int m_statusCode = 400;
	std::string m_body;
	HttpHeader m_header;
};