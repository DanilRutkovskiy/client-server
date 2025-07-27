#pragma once

#include <boost/asio.hpp>
#include <utility>
#include <algorithm>
#include "../Common/HttpHeader.h"
#include "HttpHeaderParser.h"

enum class ParserState
{
	WAITING_FOR_HEADERS,
	DONE
};

struct HttpResponsePopulator
{
	typedef boost::asio::buffers_iterator<
		boost::asio::streambuf::const_buffers_type> Iterator;

	const bool done = true;
	const bool notDone = false;

	ParserState m_state = ParserState::WAITING_FOR_HEADERS;

	std::string m_buffer;
	std::string m_delimiter = "\r\n\r\n";
	HttpHeader m_headerData;

	HttpHeader ReadHeaderData()
	{
		return m_headerData;
	}

	template <typename Iterator>
	std::pair<Iterator, bool> operator()(Iterator begin, Iterator end)
	{
		if (ParserState::DONE == m_state)
		{
			return std::make_pair(begin, done);
		}
		m_buffer.insert(m_buffer.end(), begin, end);

		if (ParserState::WAITING_FOR_HEADERS == m_state)
		{
			const auto loc = std::search(std::begin(m_buffer), std::end(m_buffer), 
				std::begin(m_delimiter), std::end(m_delimiter));
			if (std::end(m_buffer) == loc)
			{
				return std::make_pair(end, notDone);
			}

			std::string headerData;
			headerData.insert(headerData.end(), std::begin(m_buffer), loc );
			m_headerData = HttpHeaderParser{}(headerData);
			//ReadHeader(headerData);
			return std::make_pair(begin, done);
		}
		//never reach here
		return std::make_pair(end, done);
	} 
};