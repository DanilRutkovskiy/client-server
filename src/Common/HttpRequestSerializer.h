#pragma once
#include <string>
#include <sstream>

#include "HttpRequest.h"

struct HttpRequestSerializer
{
	HttpRequestSerializer(const HttpRequest& httpRequest)
		:m_httpRequest{ httpRequest }
	{

	}

	std::string operator()()
	{
		std::stringstream ss;

		ss << toString(m_httpRequest.m_verb) << "  "
		   << m_httpRequest.m_path << " HTTP/1.1\r\n" 
		   << "Host: " << m_httpRequest.m_host << "\r\n\r\n";

		return ss.str();
	}

private:
	const HttpRequest& m_httpRequest;
};