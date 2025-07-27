#pragma once
#include "../Common/HttpHeader.h"
#include <string>
#include "boost/algorithm/string.hpp"

struct HttpHeaderParser
{
	HttpHeader operator()(const std::string& data)
	{
		std::vector<std::string> lines;
		boost::split(lines, data, boost::is_any_of("\r\n"));
		if (lines.empty())
		{
			return {};
		}

		const auto& startLine = lines[0];

		HttpHeader httpHeader;
		boost::split(httpHeader.m_startLine.parts, startLine, boost::is_any_of(" "));

		lines.erase(lines.begin());

		for (const auto& line : lines)
		{

		}

		return httpHeader;
	}
};