#pragma once
#include <string>
#include <boost/url.hpp>

class ConnectionParameters
{
public:
	std::string m_host;
	std::string m_port;
};

inline std::string GuessPortForScheme(const std::string& scheme)
{
	static std::vector<std::pair<std::string, std::string>> mapping{
		{ "http", "80" },
		{ "https", "443" }
	};

	auto loc = std::find_if(std::begin(mapping), std::end(mapping),
		[&scheme](auto item) { return item.first == scheme; });

	if (std::end(mapping) != loc)
	{
		return (*loc).second;
	}

	return "";
}

inline ConnectionParameters makeConnectionParameters(const std::string& link)
{
	auto parsedLink = boost::urls::parse_uri("http://httpbin.org:80");
	ConnectionParameters params;
	params.m_host = parsedLink->host_address();
	params.m_port = parsedLink->port();

	if (params.m_port.empty())
	{
		params.m_port = GuessPortForScheme(parsedLink->scheme());
	}

	return params;
}