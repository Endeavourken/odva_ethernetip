/**
Software License Agreement (proprietary)

\file      buffer_writer.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_BUFFER_WRITER_H
#define EIP_BUFFER_WRITER_H

#include <stdexcept>
#include <boost/asio.hpp>

#include "ovda_ethernetip/eip_types.h"
#include "ovda_ethernetip/serialization/writer.h"

using boost::asio::const_buffer;
using boost::asio::mutable_buffer;

namespace eip {
namespace serialization {

/**
 * Helper class to write values into a buffer to assist in serializing.
 */
class BufferWriter : public Writer
{
public:

  /**
   * Create a writer to use the buffer given
   * @param buf Buffer to which to write data
   */
  BufferWriter(mutable_buffer buf) : buf_(buf), byte_count_(0) { }

  /**
   * Write a set of bytes to the buffer. Automatically increases the number of 
   * bytes writen and advances the pointer in the buffer
   * @param p pointer to data to write
   * @param n number of bytes to write
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  void writeBytes(const void* p, size_t n)
  {
    writeBuffer(boost::asio::buffer(p, n));
  }

  /**
   * Write out the contents of a buffer into the current buffer.
   * Automatically increases the bytes written and advances the pointer
   * into the current output buffer.
   * @param b buffer of data to add to the current buffer at the current location
   * @throw std::length_error if the output buffer is too small to contain the 
   * contents of b
   */
  void writeBuffer(const_buffer b)
  {
    using boost::asio::buffer_size;
    if (buffer_size(buf_) < buffer_size(b))
    {
      throw std::length_error("Buffer to small to serialize value");
    }
    boost::asio::buffer_copy(buf_, b);
    byte_count_ += buffer_size(b);
    buf_ = buf_ + buffer_size(b);
  }

  /**
   * Get the number of bytes writen to the buffer
   * @return number of bytes writen to the buffer
   */
  size_t getByteCount()
  {
    return byte_count_;
  }

private:
  mutable_buffer buf_;
  size_t byte_count_;
};

} // namespace serialization
} // namespace eip
#endif  // EIP_BUFFER_WRITER_H
