#pragma once
#include <boost/asio.hpp>
#include "../HttpClient/HttpClient.h"

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
	}
};