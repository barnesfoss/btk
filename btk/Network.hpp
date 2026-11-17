/*
 * Copyright Kameron Barnes
 * SPDX-License-Identifier: MIT
 * Part of the btk library
 *
 * File: Network.hpp
 * Cross-platform network utilities
 */

// TODO.kb Write documentation
#ifdef _WIN32
// Windows because Windows has to be *different*
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <winsock2.h>
#include <ws2tcpip.h>
// If we are running under MSVC we can automatically link winsock2
#if defined(_MSC_VER) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
#pragma comment(lib, "Ws2_32.lib")

// NIGHTMARE NIGHTMARE NIGHTMARE
#ifndef _SSIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64 ssize_t;
#else
typedef _W64 unsigned int ssize_t;
#endif
#define _SSIZE_T_DEFINED
#endif

#endif

#else
// *nix
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

// Common headers
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef BTK_NETWORK_HPP
#define BTK_NETWORK_HPP

namespace btk {
// Define varying structures & functions on Windows & *nix into "common" representation
#ifdef _WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif

inline void closeSocket(socket_t socket) {
#ifdef _WIN32
  closesocket(socket);
#else
  close(socket);
#endif
}

inline int checkSocketError(socket_t socket) {
#ifdef _WIN32
  return socket == INVALID_SOCKET;
#else
  return socket < 0;
#endif
}

enum SocketFamily { IPv4 = AF_INET, IPv6 = AF_INET6, IPvX = AF_IPX };

enum SocketType { TCP = SOCK_STREAM };

// Socket is raw socket calls put into an OOP abstraction
class Socket {
public:
  Socket() : Socket(IPv4, TCP, 0) {}

  Socket(int domain, int type, int protocol) {
    // Windows requires us to make a WSA
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
      fprintf(stderr, "WSAStartup failed\n");
    }
#endif
    sock = socket(domain, type, protocol);
    if (checkSocketError(sock)) {
      throw std::runtime_error("socket creation failed\n");
#ifdef _WIN32
      WSACleanup();
#endif
      return;
    }
  }

  Socket(socket_t sock) : sock(sock) {}

  ~Socket() {
    close();
  }

  void setOption(int option_name, const void *option_value, socklen_t value_size) {
#ifdef _WIN32
    setsockopt(sock, SOL_SOCKET, option_name, static_cast<const char *>(option_value), value_size);
#else
    setsockopt(sock, SOL_SOCKET, option_name, option_value, value_size);
#endif
  };

  void setOption(int option_name, int value) {
    setOption(option_name, &value, sizeof(value));
  }

  int bind(struct sockaddr_in addr) {
    return ::bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  }

  void listen(int backlog = SOMAXCONN) {
    ::listen(sock, backlog);
  }

  ssize_t send(const char *data, size_t size) {
#ifdef _WIN32
    // Winsock uses int for the buffer length.
    int chunk = size > INT_MAX ? INT_MAX : (int)size;
    int n = ::send(sock, data, chunk, 0);
#else
    // POSIX send() returns ssize_t.
    ssize_t n = ::send(sock, data, size, 0);
#endif
    return n;
  }

  ssize_t send(const std::string data) {
    return send(data.c_str(), data.length());
  }

  bool sendAll(const char *data, size_t size) {
    // Sockets are allowed to send less than requested
    // So we loop until all data is sent
    while (size > 0) {
      size_t n = send(data, size);
      if (n <= 0)
        return false;

      data += n;
      size -= static_cast<size_t>(n);
    }

    return true;
  }

  bool sendAll(const std::string data) {
    return sendAll(data.c_str(), data.size());
  }

  void close() {
    closeSocket(sock);

    // Windows...
#ifdef _WIN32
    WSACleanup();
#endif
  }

  Socket accept(sockaddr *addr = nullptr, socklen_t *addrLen = nullptr) {
    return ::accept(sock, addr, addrLen);
  }

  std::string request(size_t size = 4096) {
    std::vector<char> buffer(size);

    ssize_t bytesReceived = ::recv(sock, buffer.data(), buffer.size(), 0);

    if (bytesReceived == -1)
      throw std::runtime_error("request() failed");

    return std::string(buffer.data(), bytesReceived);
  }

  socket_t internal() {
    return sock;
  }

  int error() {
    return checkSocketError(sock);
  }

  int connect(const sockaddr *address, size_t addressSize) {
    return ::connect(sock, address, addressSize);
  }

  int connect(std::string host, uint16_t port = 80) {
    struct addrinfo hints, *res, *p;

    // DNS Resolution hints
    hints.ai_family = IPv4;
    hints.ai_socktype = TCP;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    // Name resolution
    int err = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res);
    if (err != 0) {
      std::cerr << "getaddrinfo failed: " << gai_strerror(err) << '\n';
      return -1;
    }

    // Loop through results and try to connect
    for (p = res; p != NULL; p = p->ai_next) {
      Socket sock(p->ai_family, p->ai_socktype, p->ai_protocol);
      if (error())
        continue;
      if (connect(p->ai_addr, p->ai_addrlen) == 0) {
        break;
      }
      close();
    }

    if (p == NULL) {
      freeaddrinfo(res);
      return -1;
    }

    freeaddrinfo(res);
    return 0;
  }

  ssize_t read(char *buffer, size_t bufferSize, int flags = 0) {
    return ::recv(sock, buffer, bufferSize, flags);
  }

  ssize_t read(std::vector<char> &buffer, int flags = 0) {
    buffer.clear();
    return read(buffer.data(), buffer.capacity(), flags);
  }

private:
#ifdef _WIN32
  WSADATA wsa;
#endif
  socket_t sock;
};

class Port : public Socket {
public:
  Port(uint16_t out, SocketFamily IPType = IPv4, int inAddr = INADDR_ANY) : Socket() {
    struct sockaddr_in addr;
    addr.sin_family = IPType;
    addr.sin_addr.s_addr = htonl(inAddr);
    addr.sin_port = htons(out);
    if (bind(addr) > 0) {
      close();
      throw std::runtime_error("bind() failed\n");
    }
  }

  ~Port() {
    close();
  }
};
} // namespace btk
#endif
