#pragma once
#include "../Common/HttpHeader.h"
#include <string>
#include <tuple>
#include "boost/algorithm/string.hpp"

struct HttpHeaderParser
{
	std::pair<bool, HttpHeader> operator()(const std::string& data)
	{
		std::vector<std::string> lines;
		boost::split(lines, data, boost::is_any_of("\r\n"), boost::token_compress_on);
		if (lines.empty())
		{
			return std::make_pair(false, HttpHeader{});
		}

		const auto& startLine = lines[0];

		HttpHeader httpHeader;
		boost::split(httpHeader.m_startLine.parts, startLine, boost::is_any_of(" "));
		if (httpHeader.m_startLine.parts.size() < 3)//TODO change 3 to something else
		{
			return std::make_pair(false, HttpHeader{});
		}

		lines.erase(lines.begin());

		for (const auto& line : lines)
		{
			if (line.empty())
			{
				continue;
			}
			httpHeader.Add(ParseHeaderLine(line));
		}

		return std::make_pair(true, std::move(httpHeader));
	}

	HeaderField ParseHeaderLine(const std::string& line)
	{
		HeaderField headerField;

		auto loc = std::find(std::begin(line), std::end(line), ':');
		if (loc == std::end(line))
		{
			headerField.m_name = line;
		}
		else
		{
			headerField.m_name = std::string{ std::begin(line), loc };
			headerField.m_value = std::string{ ++loc, end(line) };
		}

		boost::algorithm::trim(headerField.m_name);
		boost::algorithm::trim(headerField.m_value);

		return headerField;
	}
};