#pragma once

#include <boost/asio.hpp>

struct HttpClientParameters
{
	boost::asio::any_io_executor m_executor;
};