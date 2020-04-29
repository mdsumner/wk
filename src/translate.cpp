
#include <sstream>
#include <Rcpp.h>
#include "wk/wkt-writer.h"
#include "wk/wkb-writer.h"
#include "wk/io-rcpp.h"

using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::CharacterVector cpp_translate_wkb_wkt(Rcpp::List wkb, int includeZ, int includeM,
                                            int includeSRID, int precision, bool trim) {
  WKRawVectorListProvider provider(wkb);
  WKCharacterVectorExporter exporter(provider.nFeatures());

  WKTWriter writer(exporter);
  WKBReader reader(provider, writer);

  writer.setIncludeZ(includeZ);
  writer.setIncludeM(includeM);
  writer.setIncludeSRID(includeSRID);
  exporter.setRoundingPrecision(precision);
  exporter.setTrim(trim);

  writer.hasNextFeature();
  while (reader.hasNextFeature()) {
    reader.iterateFeature();
    writer.hasNextFeature();
  }

  return exporter.output;
}

// [[Rcpp::export]]
Rcpp::List cpp_translate_wkb_wkb(Rcpp::List wkb, int includeZ, int includeM,
                                 int includeSRID, int endian, int bufferSize) {

  WKRawVectorListProvider provider(wkb);
  WKRawVectorListExporter exporter(provider.nFeatures());

  WKBWriter writer(exporter);
  WKBReader reader(provider, writer);

  exporter.setBufferSize(bufferSize);
  writer.setIncludeZ(includeZ);
  writer.setIncludeM(includeM);
  writer.setIncludeSRID(includeSRID);
  writer.setEndian(endian);

  writer.hasNextFeature();
  while (reader.hasNextFeature()) {
    reader.iterateFeature();
    writer.hasNextFeature();
  }

  return exporter.output;
}
