
#ifndef WK_FORMAT_H
#define WK_FORMAT_H

#include "wk/geometry-handler.h"
#include "wk/wkb-reader.h"
#include "wk/wkt-streamer.h"
#include "wk/wkt-writer.h"

class WKMaxCoordinatesException: public WKParseException {
public:
  static const int CODE_HAS_MAX_COORDS = 32453;
  WKMaxCoordinatesException(): WKParseException(CODE_HAS_MAX_COORDS) {}
};


class WKFormatter: public WKTWriter {
public:
  WKFormatter(WKStringExporter& exporter, int maxCoords):
  WKTWriter(exporter), maxCoords(maxCoords), thisFeatureCoords(0) {}

  void nextFeatureStart(size_t featureId) {
    this->thisFeatureCoords = 0;
    WKTWriter::nextFeatureStart(featureId);
  }

  void nextCoordinate(const WKGeometryMeta& meta, const WKCoord& coord, uint32_t coordId) {
    WKTWriter::nextCoordinate(meta, coord, coordId);
    this->thisFeatureCoords++;
    if (this->thisFeatureCoords >= this->maxCoords) {
      throw WKMaxCoordinatesException();
    }
  }

  bool nextError(WKParseException& error, size_t featureId) {
    if (error.code() == WKMaxCoordinatesException::CODE_HAS_MAX_COORDS) {
      this->exporter.writeConstChar("...");
    } else {
      this->exporter.writeConstChar("!!! ");
      this->exporter.writeConstChar(error.what());
    }

    this->nextFeatureEnd(featureId);
    return true;
  }

private:
  int maxCoords;
  int thisFeatureCoords;
};

#endif
