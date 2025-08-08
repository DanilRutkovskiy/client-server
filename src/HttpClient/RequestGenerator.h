#pragma once

#include "../Common//HttpRequest.h"

#include <boost/url.hpp>

class RequestCreator
{
public:
	static HttpRequest makeGetRequest(const std::string& link)
	{
		auto parsedLink = boost::urls::parse_uri(link);

		HttpRequest request;
		request.m_host = parsedLink->host_address();
		request.m_verb = HttpVerb::GET;
		request.m_path = parsedLink->path();

		return request;
	}
};

