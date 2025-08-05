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
	
public:
	explicit StressTester(boost::asio::io_context& ioContext)
		:
		m_executor( ioContext.get_executor() )
	{}
	
	void operator()()
	{
	}
};