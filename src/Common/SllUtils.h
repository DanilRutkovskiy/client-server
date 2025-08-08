#pragma once
#include <memory>
#include <boost/asio/ssl.hpp>

std::shared_ptr<boost::asio::ssl::context> PrepareSslContext()
{
	using namespace boost::asio::ssl;
	auto sslContext = std::make_shared<context>(context::tlsv12);

	sslContext->load_verify_file("cacert.pem");
	sslContext->set_verify_mode(verify_peer);

	sslContext->set_verify_callback([](bool preverified, verify_context& verifyContext)
		{
			return preverified;
		});

	const char* defaultCipherList = "HIGH:!ADH:!MD5:!RC4:!SRP:!PSK:!DSS";
	const auto ret = SSL_CTX_set_cipher_list(sslContext->native_handle(), defaultCipherList);
	if (!ret)
	{
		throw ("can't set cipher list");
	}

	auto options = context::default_workarounds |
		context::no_sslv2 |
		context::no_sslv3 |
		context::no_tlsv1 |
		context::no_tlsv1_1;

	sslContext->set_options(options);

	return sslContext;
}