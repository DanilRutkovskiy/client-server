#pragma once
#include <boost/asio.hpp>
#include "../HttpClient/HttpClient.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"


class Tester
{
	boost::asio::any_io_executor m_executor;

public:
	explicit Tester(boost::asio::io_context& ioContext)
		:
		m_executor( ioContext.get_executor() )
	{}
	
	void operator()() 
	{
		HttpClientParameters parameters;
		parameters.m_executor = m_executor;

		auto client = HttpClient::Make(std::move(parameters));

		ConnectionParameters connParams;
		client->ConnectAsync(std::move(connParams), [](std::error_code err) {});

		HttpRequest request;
		client->SendAsync(request, [](std::error_code err, HttpResponse response) 
			{
				if (err)
				{
					std::cout << "error occured: " << err.message() << std::endl;
					return;
				}

				std::cout << "Request succeeded. \n" 
					"Response code: " << response.m_statusCode << std::endl <<
					"Response body: " << response.m_body << std::endl;
			});
	}
};