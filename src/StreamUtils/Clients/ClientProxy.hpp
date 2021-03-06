// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2021
// MIT License

#pragma once

#include <Client.h>

#include "../Configuration.hpp"
#include "../Streams/StreamProxy.hpp"

namespace StreamUtils {

template <typename ReadPolicy, typename WritePolicy, typename ConnectPolicy>
class ClientProxy : public Client {
 public:
  explicit ClientProxy(Client &upstream, ReadPolicy reader = ReadPolicy(),
                       WritePolicy writer = WritePolicy(),
                       ConnectPolicy connection = ConnectPolicy())
      : _target(upstream),
        _reader(reader),
        _writer(Polyfills::move(writer)),
        _connection(connection) {}

  ClientProxy(const ClientProxy &other)
      : _target(other._target),
        _reader(other._reader),
        _writer(other._writer),
        _connection(other._connection) {}

  ~ClientProxy() {
    _writer.implicitFlush(_target);
  }

  // --- Print ---

  size_t write(const uint8_t *buffer, size_t size)   {
    return _writer.write(_target, buffer, size);
  }

  size_t write(uint8_t data)   {
    return _writer.write(_target, data);
  }

  using Print::write;

  // --- Stream ---

  int available()   {
    return _reader.available(_target);
  }

  int read()   {
    return _reader.read(_target);
  }

  int peek()   {
    return _reader.peek(_target);
  }

#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
  size_t readBytes(char *buffer, size_t size)   {
    return _reader.readBytes(_target, buffer, size);
  }
#endif

  // --- Client ---

  int connect(IPAddress ip, uint16_t port)   {
    return _connection.connect(_target, ip, port);
  }

  int connect(const char *ip, uint16_t port)   {
    return _connection.connect(_target, ip, port);
  }

  uint8_t connected()   {
    return _connection.connected(_target);
  }

  void stop()   {
    _writer.implicitFlush(_target);
    _connection.stop(_target);
  }

  operator bool()   {
    return _connection.operator_bool(_target);
  }

  int read(uint8_t *buf, size_t size)   {
    return _reader.read(_target, buf, size);
  }

  void flush()   {
    _writer.flush(_target);
  }

 protected:
  Client &_target;
  ReadPolicy _reader;
  WritePolicy _writer;
  ConnectPolicy _connection;
};

}  // namespace StreamUtils