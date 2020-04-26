
#ifndef WKHEADERS_WKB_WKB_TRANSLATOR
#define WKHEADERS_WKB_WKB_TRANSLATOR

#include "wkheaders/wkb-writer.h"
#include "wkheaders/wkb-reader.h"

class WKBWKBTranslator: WKBReader {
public:
  WKBWKBTranslator(BinaryReader& reader, BinaryWriter& writer): WKBReader(reader), writer(writer) {

  }

  // expose these as the public interface
  virtual bool hasNextFeature() {
    this->writer.seekNextFeature();
    return WKBReader::hasNextFeature();
  }

  virtual void iterateFeature() {
    WKBReader::iterateFeature();
  }

  void setEndian(unsigned char endian) {
    this->writer.setEndian(endian);
  }

  void setIncludeSRID(int includeSRID) {
    this->includeSRID = includeSRID;
  }

  void setIncludeZ(int includeZ) {
    this->includeZ = includeZ;
  }

  void setIncludeM(int includeM) {
    this->includeM = includeM;
  }

protected:
  WKBWriter writer;

  virtual void nextFeature(size_t featureId) {
    WKBReader::nextFeature(featureId);
  }

  virtual void nextNull(size_t featureId) {
    this->writer.writeNull();
  }

  void nextGeometry(const GeometryType geometryType, uint32_t partId, uint32_t size) {
    this->writer.writeEndian();

    uint32_t ewkbType = geometryType.ewkbType;
    this->writer.writeUint32(ewkbType);

    if (geometryType.simpleGeometryType != SimpleGeometryType::Point) {
      this->writer.writeUint32(size);
    }

    WKBReader::nextGeometry(geometryType, partId, size);
  }

  void nextLinearRing(const GeometryType geometryType, uint32_t ringId, uint32_t size) {
    this->writer.writeUint32(size);
    WKBReader::nextLinearRing(geometryType, ringId, size);
  }

  void nextCoordinate(const WKCoord coord, uint32_t coordId) {
    this->writer.writeCoord(coord);
  }

private:
  int includeSRID;
  int includeZ;
  int includeM;
};

#endif
