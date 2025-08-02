#pragma once
#include <string>

enum class HttpVerb
{
	GET,
	POST
};

inline std::string toString(HttpVerb verb)
{
	switch (verb)
	{
	case HttpVerb::GET:
		return "GET";
	case HttpVerb::POST:
		return "POST";
	default:
		return "";
	};
}

struct HttpRequest
{
	HttpVerb m_verb;
	std::string m_path;
	std::string m_host;
};

