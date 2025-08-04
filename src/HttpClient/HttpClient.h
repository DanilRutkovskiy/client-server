#pragma once
#include <memory>
#include <string>
#include <boost/asio/ssl.hpp>

#include "HttpClientParameters.h"
#include "ConnectionParameters.h"
#include "../Common/HttpRequest.h"
#include "../Common/HttpResponse.h"
#include "HttpResponsePopulator.h"
#include "HttpResponseStreamParser.h"
#include "../Common/HttpRequestSerializer.h"

class HttpClient : public std::enable_shared_from_this<HttpClient>
{
public:
	static std::shared_ptr<HttpClient> Make(HttpClientParameters parameters)
	{
		return std::shared_ptr<HttpClient>(new HttpClient(std::move(parameters)));
	}

	template<typename Callable>
	void ConnectAsync(ConnectionParameters connectionParams, Callable callable)
	{
		m_useTls = connectionParams.m_useTls;

		m_resolver.async_resolve	
		(
			connectionParams.m_host, connectionParams.m_port,
			[callable = std::move(callable), sharedThis = this->shared_from_this(), this, host = connectionParams.m_host]
			(boost::system::error_code err, const boost::asio::ip::tcp::resolver::results_type& endpoints) mutable
			{
				if (err)
				{
					std::cout << "resolve error oocured: " << err.message() << std::endl;
					callable(err);
					DeferDeletion();
					return;
				}

				if (m_useTls)
				{
					if (!SSL_set_tlsext_host_name(m_tlsSocket->native_handle(), host.c_str()))
					{
						callable(std::make_error_code(static_cast<std::errc>(static_cast<int>(::ERR_get_error()))));
						return;
					}

					auto& socket = sharedThis->m_tlsSocket;
					boost::asio::async_connect(socket->lowest_layer(), endpoints,
						[callable = std::move(callable), sharedThis = std::move(sharedThis), this]
						(boost::system::error_code err, boost::asio::ip::tcp::endpoint ep)
						{
							if (err)
							{
								std::cout << "connect error occured: " << err.message() << std::endl;
								callable(err);
								DeferDeletion();
							}

							m_tlsSocket->async_handshake(boost::asio::ssl::stream_base::client, 
								[callable = std::move(callable), this](boost::system::error_code err)
								{
									callable(err);
									DeferDeletion();
								});

							DeferDeletion();
						});
				}
				else
				{
					auto& socket = sharedThis->m_socket;
					boost::asio::async_connect(socket, endpoints,
						[callable = std::move(callable), sharedThis = std::move(sharedThis), this]
						(boost::system::error_code err, boost::asio::ip::tcp::endpoint ep)
						{
							if (err)
							{
								std::cout << "connect error occured: " << err.message() << std::endl;
								callable(err);
								DeferDeletion();
							}

							callable(std::error_code{});
							DeferDeletion();
						});
				}
			}
		);
	}

	template<typename Callable>
	void SendAsync(const HttpRequest& httpRequest, Callable callable)
	{
		ReadResponseAsync(m_socket, std::move(callable));
		SendMessageAsync(m_socket, HttpRequestSerializer{ httpRequest }());
	}
	 
protected:
	explicit HttpClient(HttpClientParameters parameters)
		:
		m_parameters(std::move(parameters)),
		m_resolver(m_parameters.m_executor),
		m_socket(m_parameters.m_executor),
		m_parser(&m_populator)
	{
		if (m_parameters.m_sslContext)
		{
			using namespace boost::asio;
			m_tlsSocket = std::make_shared<ssl::stream<ip::tcp::socket>>
				(
					m_parameters.m_executor,
					*m_parameters.m_sslContext
				);
		}
	}

	template<typename Callable>
	void ReadResponseAsync(boost::asio::ip::tcp::socket& socket, Callable callable)
	{
		boost::asio::async_read_until(socket, m_response, m_parser, 
			[sharedThis = this->shared_from_this(), callable = std::move(callable), this]
			(boost::system::error_code err, size_t byteContt) mutable
			{
				if (err)
				{
					callable(err, HttpResponse{});
				}
				//std::string data{ std::istreambuf_iterator<char>(&m_response), std::istreambuf_iterator<char>() };

				callable(std::error_code{}, m_populator.ResponseData());
			});
	}

	void SendMessageAsync(boost::asio::ip::tcp::socket& socket, const std::string& content)
	{
		std::ostream os(&m_request);
		os << content;

		boost::asio::async_write(socket, m_request, 
			[sharedThis = this->shared_from_this()]
			(boost::system::error_code err, std::size_t bytes_transfered)
			{
				//TODO handle error here
			});
	}

	void DeferDeletion()
	{
		post(m_parameters.m_executor,
			[sharedThis = this->shared_from_this()]() mutable
			{
				sharedThis.reset();
			});
	}

protected:
	HttpClientParameters m_parameters;
	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::ip::tcp::socket m_socket;
	std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_tlsSocket;
	boost::asio::streambuf m_request;
	boost::asio::streambuf m_response;
	HttpResponsePopulator m_populator;
	HttpResponseStreamParser m_parser;
	bool m_useTls = false;
};