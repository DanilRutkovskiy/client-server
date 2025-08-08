#define CATCH_MAIN_CONFIG

#include "../../../../libs/thirdparty/include/catch2/catch_amalgamated.hpp"
#include "../HttpClient/ConnectionParameters.h"
#include "../HttpClient/RequestGenerator.h"

TEST_CASE("Http Connection Parameters Parsing")
{
	auto p1 = makeConnectionParameters("http://whatever.com/getrequest/path");
	CHECK(p1.m_host == "whatever.com");
	CHECK(p1.m_port == "80");

	auto p2 = makeConnectionParameters("http://whatever.com:9812/getrequest/path");
	CHECK(p2.m_port == "9812");

	auto p3 = makeConnectionParameters("https://whatever.com/getrequest/path");
	CHECK(p3.m_port == "443");
}

TEST_CASE("Get Request Generate Parsing")
{
	const auto r1 = RequestCreator::makeGetRequest("http://httpbin.org/get");
	CHECK(r1.m_host == "httpbin.org");
	CHECK(r1.m_verb == HttpVerb::GET);
	CHECK(r1.m_path == "/get");
}