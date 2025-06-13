#pragma once
#include <boost/asio.hpp>

struct Tester
{
	boost::asio::io_context& m_ioContext;

	explicit Tester(boost::asio::io_context& ioContext)
		:
		m_ioContext( ioContext )
	{}
	
	void operator()() {};
};