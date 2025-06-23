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

	template<typename Callable>
	void ConnectAsync(ConnectionParameters connectionParams, Callable callable)
	{
		m_resolver.async_resolve(
			connectionParams.m_host, connectionParams.m_port,
			[callable = std::move(callable)]
			(boost::system::error_code err, const boost::asio::ip::tcp::resolver::results_type& endpoints) mutable
			{
				if (err)
				{
					std::cout << "error: " << err.message() << std::endl;
					return callable(err);
				}

				std::cout << "successfully resolved" << std::endl;
				callable(std::error_code());
			}
		);
	}

	template<typename Callable>
	void SendAsync(const HttpRequest& request, Callable callable)
	{
		post(m_parameters.m_executor, 
			[callable = std::move(callable)]() mutable
			{
				HttpResponse response{};
				response.m_body = "glad to know you";
				callable(std::error_code(), std::move(response));
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