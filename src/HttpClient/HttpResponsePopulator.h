#pragma once

#include <boost/asio.hpp>
#include <utility>

struct HttpResponsePopulator
{
	typedef boost::asio::buffers_iterator<
		boost::asio::streambuf::const_buffers_type> Iterator;

	const bool done = true;
	const bool notDone = false;

	template <typename Iterator>
	std::pair<Iterator, bool> operator()(Iterator begin, Iterator end)
	{
		const auto size = std::distance(begin, end);
		if (size > 100)
		{
			return std::make_pair(end, done);
		}
		else
		{
			return std::make_pair(end, notDone);
		}
	}
};