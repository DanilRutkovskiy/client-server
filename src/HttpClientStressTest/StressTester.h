#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "../HttpClient/HttpClient.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"
#include "../HttpClient/RequestGenerator.h"


class StressTester
{
	boost::asio::any_io_executor m_executor;
	std::shared_ptr<boost::asio::ssl::context> m_sslContext;
	int m_concurrency = 0;
	
public:
	explicit StressTester(boost::asio::io_context& ioContext, 
		const std::shared_ptr<boost::asio::ssl::context>& sslContext,
		int concurrency)
		:
		m_executor{ ioContext.get_executor() },
		m_sslContext{ sslContext },
		m_concurrency{ concurrency }
	{}

	void operator()()
	{
		for (int i = 0; i < m_concurrency; ++i)
		{
			Start();
		}
	}
	
	void Start()
	{
		HttpClientParameters parameters;
		parameters.m_executor = m_executor;
		parameters.m_sslContext = m_sslContext;

		auto client = HttpClient::Make(std::move(parameters));

		std::string link = "http://127.0.0.1/a.txt";

		auto connectionsParams = makeConnectionParameters(link);

		client->ConnectAsync(std::move(connectionsParams),
			[client, link, this](std::error_code err)
			{
				if (err)
				{
					std::cout << "ConnectAsync error occured: " << err.message() << std::endl;
					return;
				}
				std::cout << "ConnectAsync success" << std::endl;

				auto request = makeGetRequest(link);

				client->SendAsync(request, 
					[this](std::error_code err, HttpResponse response)
					{
						if (err)
						{
							std::cout << "error occured: " << err.message() << std::endl;
							return;
						}

						auto valid = Validate(response);
						if (valid)
						{
							std::cout << "Request succeded\n";
						}
						else
						{
							std::cout << "Request failed\n";
						}
					});
			});
	}

	bool Validate(const HttpResponse& response)
	{
		if (response.GetStatusCode() == 200)
		{
			return true;
		}

		return false;
	}
};