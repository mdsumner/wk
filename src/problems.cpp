
#include <Rcpp.h>
#include "wk/io-rcpp.h"
#include "wk/geometry-handler.h"
#include "wk/wkb-reader.h"
using namespace Rcpp;

class RcppWKBValidator: WKGeometryHandler {
public:
  Rcpp::CharacterVector output;
  WKBReader reader;

  RcppWKBValidator(WKRawVectorListProvider& reader): reader(reader, *this) {
    this->output = CharacterVector(reader.nFeatures());
  }

  // expose these as the public interface
  virtual bool hasNextFeature() {
    return reader.hasNextFeature();
  }

  virtual void iterateFeature() {
    reader.iterateFeature();
  }

  virtual bool nextError(WKParseException& error, size_t featureId) {
    this->output[featureId] = error.what();
    return true;
  }

  virtual void nextFeatureEnd(size_t featureId) {
    this->output[featureId] = NA_STRING;
  }
};

// [[Rcpp::export]]
Rcpp::CharacterVector cpp_problems_wkb(Rcpp::List wkb) {
  WKRawVectorListProvider reader(wkb);
  RcppWKBValidator validator(reader);

  while (validator.hasNextFeature()) {
    validator.iterateFeature();
  }

  return validator.output;
}
