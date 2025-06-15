#pragma once
#include <memory>
#include <string>
#include "HttpClientParameters.h"
#include "ConnectionParameters.h"
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
	void ConnectAsync(ConnectionParameters request, Func func)
	{	
		m_resolver.async_resolve(
			"google.com", "80", 
			[](boost::system::error_code err, const boost::asio::ip::tcp::resolver::results_type& endpoints) 
			{
				if (err)
				{
					std::cout << "error: " << err.message() << std::endl;
				}
				else
				{
					std::cout << "successfully resolved" << std::endl;
				}
			}
		);
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
		m_parameters(std::move(parameters)),
		m_resolver(m_parameters.m_executor)
	{

	}

	HttpClientParameters m_parameters;
	boost::asio::ip::tcp::resolver m_resolver;
};