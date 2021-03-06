// StreamUtils - github.com/bblanchon/ArduinoStreamUtils
// Copyright Benoit Blanchon 2019-2021
// MIT License

#pragma once

#include <Stream.h>
#include "../Polyfills.hpp"

namespace StreamUtils {

template <typename ReadPolicy, typename WritePolicy>
class StreamProxy : public Stream {
 public:
  explicit StreamProxy(Stream &upstream, ReadPolicy reader = ReadPolicy{},
                       WritePolicy writer = WritePolicy{})
      : _upstream(upstream),
        _reader(reader),
        _writer(Polyfills::move(writer)) {}

  StreamProxy(const StreamProxy &other)
      : _upstream(other._upstream),
        _reader(other._reader),
        _writer(other._writer) {}

  ~StreamProxy() {
    _writer.implicitFlush(_upstream);
  }

  size_t write(const uint8_t *buffer, size_t size)   {
    return _writer.write(_upstream, buffer, size);
  }

  size_t write(uint8_t data)   {
    return _writer.write(_upstream, data);
  }

  using Stream::write;

  int available()   {
    return _reader.available(_upstream);
  }

  int read()   {
    return _reader.read(_upstream);
  }

  int peek()   {
    return _reader.peek(_upstream);
  }

  void flush()   {
    _writer.flush(_upstream);
  }

#if STREAMUTILS_STREAM_READBYTES_IS_VIRTUAL
  size_t readBytes(char *buffer, size_t size)   {
    return _reader.readBytes(_upstream, buffer, size);
  }
#endif

 protected:
  Stream &_upstream;
  ReadPolicy _reader;
  WritePolicy _writer;
};

}  // namespace StreamUtils