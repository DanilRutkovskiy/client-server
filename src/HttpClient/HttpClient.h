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
			[callable = std::move(callable), this, host = connectionParams.m_host]
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
					onTlsResolve(host, endpoints, std::move(callable));
				}
				else
				{
					onResolve(endpoints, std::move(callable));
				}

				DeferDeletion();
			}
		);
	}

	template<typename Callable>
	void SendAsync(const HttpRequest& httpRequest, Callable callable)
	{
		ReadResponseAsync(std::move(callable));
		SendMessageAsync(HttpRequestSerializer{ httpRequest }());
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
	 
	template <typename Callable>
	void ReadResponseAsync(Callable callable)
	{
		if (m_useTls && m_tlsSocket)
		{
			ReadResponseAsyncImpl(*m_tlsSocket, std::move(callable));
		}
		else
		{
			ReadResponseAsyncImpl(m_socket, std::move(callable));
		}
	}

	void SendMessageAsync(const std::string& content)
	{
		if (m_useTls && m_tlsSocket)
		{
			SendMessageAsyncImpl(*m_tlsSocket, content);
		}
		else
		{
			SendMessageAsyncImpl(m_socket, content);
		}
	}

private:
	template<typename SocketType>
	void SendMessageAsyncImpl(SocketType& socket, const std::string& content)
	{
		std::ostream os(&m_request);
		os << content;

		boost::asio::async_write(socket, m_request,
			[sharedThis = this->shared_from_this()]
			(boost::system::error_code err, std::size_t bytes_transfered)
			{
				if (err)
				{
					std::cout << "error after send message: " << err.what() << std::endl;
				}
			});
	}
	
	template<typename SocketType, typename Callable>
	void ReadResponseAsyncImpl(SocketType& socket, Callable callable)
	{
		boost::asio::async_read_until(socket, m_response, m_parser,
			[sharedThis = this->shared_from_this(), callable = std::move(callable), this]
			(boost::system::error_code err, size_t byteContt) mutable
			{
				if (err)
				{
					callable(err, HttpResponse{});
					DeferDeletion();
					return;
				}

				callable(std::error_code{}, m_populator.ResponseData());
				DeferDeletion();
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

	template<typename Callable>
	void onTlsResolve(const std::string& host, const boost::asio::ip::tcp::resolver::results_type& endpoints, Callable callable)
	{
		if (!SSL_set_tlsext_host_name(m_tlsSocket->native_handle(), host.c_str()))
		{
			callable(std::make_error_code(static_cast<std::errc>(static_cast<int>(::ERR_get_error()))));
			DeferDeletion();
			return;
		}

		boost::asio::async_connect(m_tlsSocket->lowest_layer(), endpoints,
			[callable = std::move(callable), this]
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

	template<typename Callable>
	void onResolve(const boost::asio::ip::tcp::resolver::results_type& endpoints, Callable callable)
	{
		boost::asio::async_connect(m_socket, endpoints,
			[callable = std::move(callable), this]
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