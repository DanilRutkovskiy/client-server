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
		connParams.m_host = "httpbin.org";
		connParams.m_port = "80";
		client->ConnectAsync(std::move(connParams), 
			[client](std::error_code err) 
			{
				if (err)
				{
					std::cout << "ConnectAsync error occured: " << err.message() << std::endl;
					return;
				}
				std::cout << "ConnectAsync success" << std::endl;

				HttpRequest request;
				std::string message = 
					"GET /get HTTP/1.1\r\n"
					"Host: httpbin.org\r\n"
					"\r\n";
				client->SendAsync(message, [](std::error_code err, HttpResponse response)
					{
						if (err)
						{
							std::cout << "error occured: " << err.message() << std::endl;
							return;
						}

						std::cout << "Request succeeded. \n"
							"Response code: " << response.m_statusCode << std::endl <<
							"Response body: " << response.m_body << std::endl <<
							"Response data: " << response.m_data << std::endl;
					});
			});

	}
};