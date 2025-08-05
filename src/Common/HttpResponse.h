#pragma once

#include "HttpHeader.h"

struct HttpResponse
{
	int m_statusCode = 400;
	std::string m_body;
	HttpHeader m_header;

	int GetStatusCode()
	{
		if (m_header.m_startLine.parts.size() < 2)
		{
			return -1;
		}

		try
		{
			return std::stoi(m_header.m_startLine.parts[1]);
		}
		catch(...)
		{
			return -2;
		}
	}
};