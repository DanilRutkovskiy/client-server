#pragma once

#include <boost/asio.hpp>
#include <utility>
#include <algorithm>
#include "../Common/HttpHeader.h"
#include "HttpHeaderParser.h"

enum class ParserState
{
	WAITING_FOR_HEADERS,
	WAITING_FOR_BODY,
	DONE
};

enum class TransferMethod
{
	NONE,
	CONTENT_LENGTH,
	CHUNKED
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
	HttpResponse m_httpResponse;
	TransferMethod m_transferMethod = TransferMethod::NONE;

	HttpResponse& ResponseData()
	{
		return m_httpResponse;
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

			std::string headerData{ std::begin(m_buffer), loc };

			auto possibleHeader = HttpHeaderParser{}(headerData);
			if (!possibleHeader.first)
			{
				m_state = ParserState::DONE;
				return std::make_pair(end, done);
			}

			m_httpResponse.m_header = std::move(possibleHeader.second);

			m_transferMethod = DetermineTransferMethod(m_httpResponse.m_header);
			if (m_transferMethod == TransferMethod::NONE)
			{
				m_state = ParserState::DONE;
				return std::make_pair(end, done);
			}

			if (m_transferMethod == TransferMethod::CHUNKED)
			{
				//TODO: support chunked transfer
				m_state = ParserState::DONE;
				return std::make_pair(end, done);
			}

			m_state = ParserState::WAITING_FOR_BODY;

			const auto bodySize = std::stoi(m_httpResponse.m_header.Get("Content-Length")->m_value);
			const auto expectedMessageSize = headerData.size() + bodySize + m_delimiter.size();
			if (expectedMessageSize > m_buffer.size())
			{
				return std::make_pair(end, notDone);
			}

			auto beginOfBody = std::begin(m_buffer) + headerData.size() + m_delimiter.size();
			auto endOfBody = std::begin(m_buffer) + expectedMessageSize;

			m_httpResponse.m_body = std::string{ beginOfBody, endOfBody };

			m_state = ParserState::DONE;

			return std::make_pair(begin, done);
		}
		//never reach here
		return std::make_pair(end, done);
	}

	TransferMethod DetermineTransferMethod(HttpHeader& header)
	{
		auto encoding = header.Get("Transfer-Encoding");
		if (encoding && encoding->m_value == "chunked")
		{
			return TransferMethod::CHUNKED;
		}

		const auto contentLength = header.Get("Content-Length");
		if (contentLength && !contentLength->m_value.empty())
		{
			return TransferMethod::CONTENT_LENGTH;
		}

		return TransferMethod::NONE;
	}
};