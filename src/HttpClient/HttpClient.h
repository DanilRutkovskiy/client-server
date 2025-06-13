#pragma once
#include <memory>
#include "HttpClientParameters.h"

class HttpClient
{
public:
	static std::shared_ptr<HttpClient> Make(HttpClientParameters params)
	{
		return std::make_shared<HttpClient>();
	}
};