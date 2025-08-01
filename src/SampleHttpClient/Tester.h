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

						std::cout << "Request succeeded.\n\n";

						Print(response);
					});
			});

	}

	static void Print(const HttpResponse& response)
	{
		for (const auto& startLinePart : response.m_header.m_startLine.parts)
		{
			std::cout << startLinePart << " ";
		}

		std::cout << std::endl;

		for (const auto& header : response.m_header.m_headers)
		{
			std::cout << header.m_name << ": " << header.m_value << std::endl;
		}

		std::cout << std::endl;

		std::cout << "body: \n" << response.m_body << std::endl;
	}
};