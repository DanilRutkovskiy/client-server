#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "../HttpClient/HttpClient.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"
#include "../HttpClient/RequestGenerator.h"
#include "LinkGenerator.h"


struct StressTester
{
	boost::asio::any_io_executor m_executor;
	std::shared_ptr<boost::asio::ssl::context> m_sslContext;
	int m_concurrency = 0;
	std::atomic<int> m_succededCount = 0;
	std::atomic<int> m_failedCount = 0;
	LinkGenerator m_linkGenerator;
	
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

		std::string link = m_linkGenerator();

		auto connectionsParams = makeConnectionParameters(link);

		const auto beforeConnect = std::chrono::steady_clock::now();
		client->ConnectAsync(std::move(connectionsParams),
			[client, link, this, beforeConnect](std::error_code err)
			{
				if (err)
				{
					++m_failedCount;
					std::cout << "ConnectAsync error occured: " << err.message() << std::endl;
					return;
				}
				const auto afterConnect = std::chrono::steady_clock::now();
				auto connectMSec = 
					std::chrono::duration_cast<std::chrono::milliseconds>(afterConnect - beforeConnect);
				std::cout << "Connection success in: " << connectMSec.count() << "millis" << std::endl;

				auto request = RequestCreator::makeGetRequest(link);

				client->SendAsync(request, 
					[this, afterConnect](std::error_code err, HttpResponse response)
					{
						if (err)
						{
							++m_failedCount;
							std::cout << "error occured: " << err.message() << std::endl;
							return;
						}

						const auto received = std::chrono::steady_clock::now();
						auto receiveTime =
							std::chrono::duration_cast<std::chrono::milliseconds>(received - afterConnect);
						std::cout << "Receive in: " << receiveTime.count() << "millis" << std::endl;


						auto valid = Validate(response);
						if (valid)
						{
							++m_succededCount;
						}
						else
						{
							++m_failedCount;
						}
					});
			});
	}

	void PrintResult()
	{
		std::cout
			<< "succeded count: " << m_succededCount << std::endl
			<< "failed count: " << m_failedCount << std::endl;
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