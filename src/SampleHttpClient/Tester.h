#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "../HttpClient/HttpClient.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"
#include "../HttpClient/RequestGenerator.h"


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
		parameters.m_sslContext = PrepareSslContext();

		auto client = HttpClient::Make(std::move(parameters));

		std::string link = "https://httpbin.org/get";

		auto connectionsParams = makeConnectionParameters(link);

		client->ConnectAsync(std::move(connectionsParams),
			[client, link](std::error_code err) 
			{
				if (err)
				{
					std::cout << "ConnectAsync error occured: " << err.message() << std::endl;
					return;
				}
				std::cout << "ConnectAsync success" << std::endl;

				auto request = makeGetRequest(link);

				client->SendAsync(request, [](std::error_code err, HttpResponse response)
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

	std::shared_ptr<boost::asio::ssl::context> PrepareSslContext()
	{
		using namespace boost::asio::ssl;
		auto sslContext = std::make_shared<context>(context::tlsv12);

		sslContext->load_verify_file("cacert.pem");
		sslContext->set_verify_mode(verify_peer);

		sslContext->set_verify_callback([](bool preverified, verify_context& verifyContext) 
			{
				return preverified;
			});

		const char* defaultCipherList = "HIGH:!ADH:!MD5:!RC4:!SRP:!PSK:!DSS";
		const auto ret = SSL_CTX_set_cipher_list(sslContext->native_handle(), defaultCipherList);
		if (!ret)
		{
			throw ("can't set cipher list");
		}

		auto options = context::default_workarounds | 
					   context::no_sslv2 | 
					   context::no_sslv3 |
					   context::no_tlsv1 | 
					   context::no_tlsv1_1;

		sslContext->set_options(options);

		return sslContext;
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