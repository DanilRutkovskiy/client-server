#pragma once
#include <memory>
#include "HttpClientParameters.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"

class HttpClient
{
public:
	static std::shared_ptr<HttpClient> Make(HttpClientParameters params)
	{
		return std::make_shared<HttpClient>();
	}

	template<typename Func>
	void SendAsync(const HttpRequest& request, Func func)
	{

	}

private:

};