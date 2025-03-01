#pragma once
#include <memory>
#include <functional>
class Buffer;
class TcpConnection;
class TimeStamp;

/*
   TcpServer.h和TcpConnection.h中都用到，所以独立出来
 */
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void (const TcpConnectionPtr&)>;
using CloseCallback = std::function<void (const TcpConnectionPtr&)>;
using WriteCompleteCallback = std::function<void (const TcpConnectionPtr&)>;
using MessageCallback = std::function<void (const TcpConnectionPtr&, Buffer*, TimeStamp)>;
using HighWaterMarkCallback = std::function<void (const TcpConnectionPtr&, size_t)>;