
#ifndef WK_WKB_WRITER_H
#define WK_WKB_WRITER_H

#include "wk/geometry-type.h"
#include "wk/wk-coord.h"
#include "wk/io-utils.h"

class WKBWriter {
public:

  WKBWriter(BinaryWriter& writer): writer(writer) {
    this->setEndian(IOUtils::nativeEndian());
  }

  bool seekNextFeature() {
    return this->writer.seekNextFeature();
  }

  void writeNull() {
    this->writer.writeNull();
  }

  void setEndian(unsigned char endian) {
    this->endian = endian;
    this->swapEndian = IOUtils::nativeEndian() != endian;
  }

  size_t writeEndian() {
    return this->writeChar(this->endian);
  }

  size_t writeCoord(WKCoord coord) {
    size_t bytesWritten = 0;
    for (size_t i=0; i < coord.size(); i++) {
       bytesWritten += this->writeDouble(coord[i]);
    }
    return bytesWritten;
  }

  size_t writeChar(unsigned char value) {
    return this->writer.writeCharRaw(value);
  }

  size_t writeDouble(double value) {
    if (this->swapEndian) {
      this->writer.writeDoubleRaw(IOUtils::swapEndian<double>(value));
    } else {
      this->writer.writeDoubleRaw(value);
    }
    return sizeof(double);
  }

  size_t writeUint32(uint32_t value) {
    if (this->swapEndian) {
      this->writer.writeUint32Raw(IOUtils::swapEndian<uint32_t>(value));
    } else {
      this->writer.writeUint32Raw(value);
    }
    return sizeof(uint32_t);
  }

private:
  bool swapEndian;
  unsigned char endian;
  BinaryWriter& writer;
};

#endif