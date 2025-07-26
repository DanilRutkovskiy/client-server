#pragma once
#include <boost/asio.hpp>
#include <utility>
#include "HttpResponsePopulator.h"

struct HttpResponseStreamParser
{
	HttpResponseStreamParser(HttpResponsePopulator* populator)
		: m_populator{ populator }
	{
	
	}

	typedef boost::asio::buffers_iterator<
		boost::asio::streambuf::const_buffers_type> Iterator;

	template <typename Iterator>
	std::pair<Iterator, bool> operator()(Iterator begin, Iterator end)
	{
		return (*m_populator)(begin, end);
	}

	HttpResponsePopulator* m_populator = nullptr;
};

namespace boost
{
	namespace asio
	{
		template<>
		struct is_match_condition<HttpResponseStreamParser> : public boost::true_type
		{

		};
	}
}