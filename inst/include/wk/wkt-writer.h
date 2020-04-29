
#ifndef WK_WRITER_H
#define WK_WRITER_H

#include <iostream>
#include "wk/geometry-handler.h"
#include "wk/writer.h"
#include "wk/wkb-reader.h"

class WKTWriter: WKGeometryHandler, public WKWriter {
public:

  WKTWriter(WKBytesProvider& reader, std::ostream& stream): reader(reader, *this), out(stream) {}

  // expose these as the public interface
  bool hasNextFeature() {
    return reader.hasNextFeature();
  }

  virtual void iterateFeature() {
    reader.iterateFeature();
  }

  // output stream manipulations I'd rather not remember
  // how to do again
  void ensureClassicLocale() {
    this->out.imbue(std::locale::classic());
  }

  void setRoundingPrecision(int precision) {
    this->out.precision(precision);
  }

  void setTrim(bool trim) {
    if (trim) {
      this->out.unsetf(out.fixed);
    } else {
      this->out.setf(out.fixed);
    }
  }

protected:
  WKBReader reader;
  std::ostream& out;

  // default null handling returns ""
  virtual void nextNull(size_t featureId) {

  }

private:
  WKGeometryMeta newMeta;

  void nextGeometryStart(const WKGeometryMeta& meta, uint32_t partId) {
    this->newMeta = this->getNewMeta(meta);
    this->writeGeometrySep(this->newMeta, partId, this->newMeta.srid);
    this->writeGeometryOpen(meta.size);
  }

  void nextGeometryEnd(const WKGeometryMeta& meta, uint32_t partId) {
    this->writeGeometryClose(meta.size);
  }

  void nextLinearRingStart(const WKGeometryMeta& meta, uint32_t ringId, uint32_t size) {
    this->writeRingSep(ringId);
    this->out << "(";
  }

  void nextLinearRingEnd(const WKGeometryMeta& meta, uint32_t ringId, uint32_t size) {
    this->out << ")";
  }

  void nextCoordinate(const WKGeometryMeta& meta, const WKCoord& coord, uint32_t coordId) {
    this->writeCoordSep(coordId);
    this->out << coord.x << " " << coord.y;
    if (this->newMeta.hasZ && coord.hasZ) {
      this->out << " " << coord.z;
    }
    if (this->newMeta.hasM && coord.hasM) {
      this->out << " " << coord.m;
    }
  }

  void writeGeometryOpen(uint32_t size) {
    if (size == 0) {
      this->out << "EMPTY";
    } else {
      this->out << "(";
    }
  }

  void writeGeometryClose(uint32_t size) {
    if (size > 0) {
      this->out << ")";
    }
  }

  void writeGeometrySep(const WKGeometryMeta& meta, uint32_t partId, uint32_t srid) {
    bool iterCollection = iteratingCollection();
    bool iterMulti = iteratingMulti();

    if ((iterCollection || iterMulti) && partId > 0) {
      this->out << ", ";
    }

    if(iterMulti) {
      return;
    }

    if(!iterCollection && meta.hasSRID) {
      this->out << "SRID=" << srid << ";";
    }

    this->out << meta.wktType() << " ";
  }

  void writeRingSep(uint32_t ringId) {
    if (ringId > 0) {
      this->out << ", ";
    }
  }

  void writeCoordSep(uint32_t coordId) {
    if (coordId > 0) {
      this->out << ", ";
    }
  }

  bool iteratingMulti() {
    size_t stackSize = this->reader.recursionLevel();
    if (stackSize <= 1) {
      return false;
    }

    const WKGeometryMeta nester = this->reader.lastGeometryType(-2);
    return nester.geometryType == WKGeometryType::MultiPoint ||
      nester.geometryType == WKGeometryType::MultiLineString ||
      nester.geometryType == WKGeometryType::MultiPolygon;
  }

  bool iteratingCollection() {
    size_t stackSize = this->reader.recursionLevel();
    if (stackSize <= 1) {
      return false;
    }

    const WKGeometryMeta nester = this->reader.lastGeometryType(-2);
    return nester.geometryType == WKGeometryType::GeometryCollection;
  }
};

#endif