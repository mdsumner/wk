
#ifndef WKHEADERS_WKB_ITERATOR_H
#define WKHEADERS_WKB_ITERATOR_H

#include <memory>
#include <cmath>
#include "wkheaders/geometry-type.h"
#include "wkheaders/io-utils.h"

class Coordinate {
public:
  const double x;
  const double y;
  const double z;
  const double m;
  const bool hasZ;
  const bool hasM;

  Coordinate(): x(NAN), y(NAN), z(NAN), m(NAN), hasZ(false), hasM(false) {}
  Coordinate(double x, double y, double z, double m, bool hasZ, bool hasM):
    x(x), y(y), z(z), m(m), hasZ(hasZ), hasM(hasM) {}

  const double& operator[](std::size_t idx) const { 
    switch (idx) {
    case 0: return x;
    case 1: return y;
    case 2: 
      if (hasZ) {
        return z; 
      } else if (hasM) {
        return m;
      }
    case 3:
      if (hasM) return m;
    default:
      throw std::runtime_error("Coordinate subscript out of range");
    }
  }

  const size_t size() {
    return 2 + hasZ + hasM;
  }

  static const Coordinate xy(double x, double y) {
    return Coordinate(x, y, NAN, NAN, false, false);
  }

  static const Coordinate xyz(double x, double y, double z) {
    return Coordinate(x, y, z, NAN, true, false);
  }

  static const Coordinate xym(double x, double y, double m) {
    return Coordinate(x, y, NAN, m, false, true);
  }

  static const Coordinate xyzm(double x, double y, double z, double m) {
    return Coordinate(x, y, z, m, true, true);
  }
};

class WKBIterator {

public:
  const static int RECURSION_LEVEL_INVALID = -1;
  const static uint32_t PART_ID_INVALID = UINT32_MAX;
  const static uint32_t RING_ID_INVALID = UINT32_MAX;
  const static uint32_t COORD_ID_INVALID = UINT32_MAX;
  const static uint32_t SRID_INVALID = UINT32_MAX;
  const static unsigned char ENDIAN_INVALID = 0xff;

  int recursionLevel;
  uint32_t partId;
  uint32_t ringId;
  uint32_t coordId;

  std::vector<GeometryType> stack;

  bool swapEndian;
  unsigned char endian;
  GeometryType geometryType;
  uint32_t srid;
  double x;
  double y;
  double z;
  double m;

  WKBIterator(BinaryReader* reader) {
    this->reader = std::unique_ptr<BinaryReader>(reader);
    this->swapEndian = false;
    this->recursionLevel = RECURSION_LEVEL_INVALID;
    this->partId = PART_ID_INVALID;
    this->ringId = RING_ID_INVALID;
    this->coordId = COORD_ID_INVALID;
    this->srid = SRID_INVALID;
    this->endian = ENDIAN_INVALID;
    this->stack = std::vector<GeometryType>();
  }

  virtual void nextFeature() {
    this->reader->seekNextFeature();
    this->recursionLevel = 0;
    this->partId = PART_ID_INVALID;
    this->ringId = RING_ID_INVALID;
    this->coordId = COORD_ID_INVALID;
    this->endian = ENDIAN_INVALID;
    this->srid = SRID_INVALID;
    this->stack.clear();

    this->readGeometry(PART_ID_INVALID);
  }

  void readGeometry(uint32_t partId) {
    this->endian = this->readChar();
    this->swapEndian = ((int)endian != (int)IOUtils::nativeEndian());
    this->nextEndian(this->endian, partId);

    this->geometryType = GeometryType(this->readUint32());
    this->stack.push_back(this->geometryType);
    this->nextGeometryType(this->geometryType, partId);

    if (geometryType.hasSRID) {
      this->srid = this->readUint32();
      this->nextSRID(this->geometryType, partId, this->srid);
    }

    if (geometryType.simpleGeometryType == SimpleGeometryType::Point) {
      this->nextGeometry(this->geometryType, partId, 1);
    } else {
      this->nextGeometry(this->geometryType, partId, this->readUint32());
    }

    this->stack.pop_back();
  }

  virtual void nextGeometry(GeometryType geometryType, uint32_t partId, uint32_t size) {
    switch (geometryType.simpleGeometryType) {
    case SimpleGeometryType::Point:
      this->nextPoint(geometryType);
      break;
    case SimpleGeometryType::LineString:
      this->nextLinestring(geometryType, size);
      break;
    case SimpleGeometryType::Polygon:
      this->nextPolygon(geometryType, size);
      break;
    case SimpleGeometryType::MultiPoint:
    case SimpleGeometryType::MultiLineString:
    case SimpleGeometryType::MultiPolygon:
    case SimpleGeometryType::GeometryCollection:
      this->nextCollection(this->geometryType, size);
      break;
    default:
      throw std::runtime_error(
          Formatter() << 
            "Unrecognized geometry type in WKBIterator::readGeometry(): " << 
            geometryType.simpleGeometryType
      );
    }
  }

  virtual void nextPoint(GeometryType geometryType) {
    this->readPoint(geometryType, 0);
  }

  virtual void nextLinestring(GeometryType geometryType, uint32_t size) {
    this->readLineString(geometryType, size);
  }

  virtual void nextPolygon(GeometryType geometryType, uint32_t size) {
    this->readPolygon(geometryType, size);
  }

  virtual void nextLinearRing(GeometryType geometryType, uint32_t ringId, uint32_t size) {
    this->readLinearRing(geometryType, size);
  }

  virtual void nextCollection(GeometryType geometryType, uint32_t size) {
    this->readMultiGeometry(geometryType, size);
  }

  void readPoint(GeometryType geometryType, uint32_t coordId) {
    this->x = this->readDouble();
    this->y = this->readDouble();

    if (geometryType.hasZ && geometryType.hasM) {
      this->z = this->readDouble();
      this->m = this->readDouble();
      this->nextCoordinate(Coordinate::xyzm(x, y, z, m), coordId);

    } else if (geometryType.hasZ) {
      this->z = this->readDouble();
      this->nextCoordinate(Coordinate::xyz(x, y, z), coordId);

    } else if (geometryType.hasM) {
      this->m = this->readDouble();
      this->nextCoordinate(Coordinate::xyz(x, y, m), coordId);

    } else {
      this->nextCoordinate(Coordinate::xy(x, y), coordId);
    }
  }

  virtual void nextCoordinate(const Coordinate coord, uint32_t coordId) {
    
  }

  virtual void nextEndian(unsigned char endian, uint32_t partId) {

  }

  virtual void nextGeometryType(GeometryType geometryType, uint32_t partId) {
      
  }

  virtual void nextSRID(GeometryType geometryType, uint32_t partId, uint32_t srid) {

  }

private:
  std::unique_ptr<BinaryReader> reader;

  void readLineString(GeometryType geometryType, uint32_t size) {
    for (uint32_t i=0; i < size; i++) {
      this->coordId = i;
      this->readPoint(geometryType, i);
    }
  }

  void readLinearRing(GeometryType geometryType, uint32_t size) {
    this->readLineString(geometryType, size);
  }

  void readPolygon(GeometryType geometryType, uint32_t size) {
    uint32_t ringSize;
    for (uint32_t i=0; i<size; i++) {
      this->ringId = i;
      ringSize = this->readUint32();
      this->nextLinearRing(geometryType, i, ringSize);
    }
  }

  void readMultiGeometry(GeometryType geometryType, uint32_t size) {
    for (uint32_t i=0; i < size; i++) {
      this->partId = i;
      this->readGeometry(i);
    }
  }

  unsigned char readChar() {
    return this->readCharRaw();
  }

  double readDouble() {
    if (this->swapEndian) {
      return IOUtils::swapEndian<double>(this->readDoubleRaw());
    } else
      return this->readDoubleRaw();
  }

  double readUint32() {
    if (this->swapEndian) {
      return IOUtils::swapEndian<uint32_t>(this->readUint32Raw());
    } else
      return this->readUint32Raw();
  }

  unsigned char readCharRaw() {
    return this->reader->readCharRaw();
  }

  double readDoubleRaw() {
    return this->reader->readDoubleRaw();
  }

  uint32_t readUint32Raw() {
    return this->reader->readUint32Raw();
  }

  bool seekNextFeature() {
    return this->reader->seekNextFeature();
  }
};

#endif