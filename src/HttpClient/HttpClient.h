#pragma once
#include <memory>
#include <string>
#include "HttpClientParameters.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"

class HttpClient
{
public:
	static std::shared_ptr<HttpClient> Make(HttpClientParameters parameters)
	{
		return std::shared_ptr<HttpClient>(new HttpClient(std::move(parameters)));
	}

	template<typename Func>
	void SendAsync(const HttpRequest& request, Func func)
	{
		post(m_parameters.m_executor, [f = std::move(func)]()
			{
				HttpResponse response{};
				response.m_body = "glad to know you";
				f(std::error_code(), std::move(response));
			});
	}

protected:
	explicit HttpClient(HttpClientParameters parameters)
		:
		m_parameters(std::move(parameters))
	{

	}

	HttpClientParameters m_parameters;
};