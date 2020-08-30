
#include "wk-v1.h"
#include <stdlib.h>

char WKV1_handler_void_vector_start(const WKV1_GeometryMeta* meta, void* userData) {
  return WKV1_CONTINUE;
}

SEXP WKV1_handler_void_vector_end(const WKV1_GeometryMeta* meta, void* userData) {
  return R_NilValue;
}

char WKV1_handler_void_feature(const WKV1_GeometryMeta* meta, R_xlen_t nFeatures, R_xlen_t featureId, void* userData) {
  return WKV1_CONTINUE;
}

char WKV1_handler_void_geometry(const WKV1_GeometryMeta* meta, uint32_t nParts, uint32_t partId, void* userData) {
  return WKV1_CONTINUE;
}

char WKV1_handler_void_ring(const WKV1_GeometryMeta* meta, uint32_t nRings, uint32_t ringId, void* userData) {
  return WKV1_CONTINUE;
}

char WKV1_handler_void_coord(const WKV1_GeometryMeta* meta, const WKV1_Coord coord, uint32_t nCoords, uint32_t coordId, void* userData) {
  return WKV1_CONTINUE;
}

char WKV1_handler_void_error(R_xlen_t featureId, int code, const char* message, void* userData) {
  return WKV1_STOP;
}

void WKV1_handler_void_finalizer(void* userData) {

}

WKV1_Handler* WKV1_handler_create() {
  WKV1_Handler* handler = (WKV1_Handler*) malloc(sizeof(WKV1_Handler));
  handler->WKAPIVersion = 1;
  handler->userData = NULL;

  handler->vectorStart = &WKV1_handler_void_vector_start;
  handler->vectorEnd = &WKV1_handler_void_vector_end;

  handler->featureStart = &WKV1_handler_void_feature;
  handler->nullFeature = &WKV1_handler_void_feature;
  handler->featureEnd = &WKV1_handler_void_feature;

  handler->geometryStart = &WKV1_handler_void_geometry;
  handler->geometryEnd = &WKV1_handler_void_geometry;

  handler->ringStart = &WKV1_handler_void_ring;
  handler->ringEnd = &WKV1_handler_void_ring;

  handler->coord = &WKV1_handler_void_coord;

  handler->error = &WKV1_handler_void_error;
  handler->finalizer = &WKV1_handler_void_finalizer;

  return handler;
}

void WKV1_handler_destroy(WKV1_Handler* handler) {
  if (handler != NULL) {
    free(handler);
  }
}

void WKV1_handler_destroy_xptr(SEXP xptr) {
  WKV1_handler_destroy((WKV1_Handler*) R_ExternalPtrAddr(xptr));
}

SEXP WKV1_handler_create_xptr(WKV1_Handler* handler, SEXP tag, SEXP prot) {
  SEXP xptr = R_MakeExternalPtr(handler, tag, prot);
  R_RegisterCFinalizerEx(xptr, &WKV1_handler_destroy_xptr, TRUE);
  return xptr;
}

SEXP WKV1_error_sentinel(int code, const char* message) {
  const char* names[] = {"code", "message", ""};
  SEXP sentinel = PROTECT(Rf_mkNamed(VECSXP, names));
  Rf_setAttrib(sentinel, Rf_install("class"), Rf_mkString("wk_error_sentinel"));
  SEXP codeSEXP = PROTECT(Rf_allocVector(INTSXP, 1));
  INTEGER(codeSEXP)[0] = code;
  SET_VECTOR_ELT(sentinel, 0, codeSEXP);

  SET_VECTOR_ELT(sentinel, 1, Rf_mkString(message));
  UNPROTECT(2);
  return sentinel;
}
