
#ifndef WK_WKT_WRITER_H
#define WK_WKT_WRITER_H

#include <iostream>
#include "wk/io-string.h"
#include "wk/geometry-handler.h"
#include "wk/writer.h"
#include "wk/wkb-reader.h"

class WKTWriter: public WKWriter {
public:

  WKTWriter(WKStringExporter& exporter): WKWriter(exporter), exporter(exporter) {}

  virtual void nextFeatureStart(size_t featureId) {
    out.str("");
    out.clear();
    this->stack.clear();
    WKWriter::nextFeatureStart(featureId);
  }

  virtual void nextFeatureEnd(size_t featureId) {
    // this is a hack until I properly convert this to using
    // the string exporter interface
    exporter.writeString(out.str());
    WKWriter::nextFeatureEnd(featureId);
  }

  void nextGeometryStart(const WKGeometryMeta& meta, uint32_t partId) {
    this->stack.push_back(meta);
    this->newMeta = this->getNewMeta(meta);
    this->writeGeometrySep(this->newMeta, partId, this->newMeta.srid);
    this->writeGeometryOpen(meta.size);
  }

  void nextGeometryEnd(const WKGeometryMeta& meta, uint32_t partId) {
    this->writeGeometryClose(meta.size);
    this->stack.pop_back();
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

protected:
  WKStringExporter& exporter;
  std::stringstream out;
  std::vector<WKGeometryMeta> stack;

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

  // stack accessors
  const WKGeometryMeta lastGeometryType(int level) {
    if (level >= 0) {
      return this->stack[level];
    } else {
      return this->stack[this->stack.size() + level];
    }
  }

  const WKGeometryMeta lastGeometryType() {
    return lastGeometryType(-1);
  }

  size_t recursionLevel() {
    return this->stack.size();
  }

  bool iteratingMulti() {
    size_t stackSize = this->recursionLevel();
    if (stackSize <= 1) {
      return false;
    }

    const WKGeometryMeta nester = this->lastGeometryType(-2);
    return nester.geometryType == WKGeometryType::MultiPoint ||
      nester.geometryType == WKGeometryType::MultiLineString ||
      nester.geometryType == WKGeometryType::MultiPolygon;
  }

  bool iteratingCollection() {
    size_t stackSize = this->recursionLevel();
    if (stackSize <= 1) {
      return false;
    }

    const WKGeometryMeta nester = this->lastGeometryType(-2);
    return nester.geometryType == WKGeometryType::GeometryCollection;
  }
};

#endif
