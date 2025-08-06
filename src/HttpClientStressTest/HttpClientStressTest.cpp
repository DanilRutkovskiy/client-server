#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "StressTester.h"

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

void test()
{
	int concurrency = 500;
	int numberOfThreads = 4;

	boost::asio::io_context ioContext;
	std::cout << "started" << std::endl;
	{
		auto sslContext = PrepareSslContext();
		auto workGuard = boost::asio::make_work_guard(ioContext);

		StressTester tester(ioContext, sslContext, concurrency);
		tester();

		for (int i = 0; i < numberOfThreads; i++)
		{
			std::jthread th([&ioContext]()
				{
					try
					{
						ioContext.run();
					}
					catch (const std::exception& ex)
					{
						std::cout << "exception: " << ex.what() << std::endl;
					}
					catch (...)
					{
						std::cout << "unknown error" << std::endl;
					}
				});

			th.detach();
		}

		std::cout << "Press enter to stop the app" << std::endl;
		std::cin.get();

		tester.PrintResult();

		workGuard.reset();
		ioContext.stop();
	}

	std::cout << "done" << std::endl;
}

int main()
{
	try
	{
		test();
	}
	catch (const std::exception& ex)
	{
		std::cout << "exception: " << ex.what() << std::endl;
	}

	return 0;
}