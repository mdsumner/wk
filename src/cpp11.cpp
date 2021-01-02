// Generated by cpp11: do not edit by hand
// clang-format off

#include <cpp11/R.hpp>
#include <Rcpp.h>
using namespace Rcpp;
#include "cpp11/declarations.hpp"

// handle-wkt.cpp
SEXP wk_cpp_handle_wkt(SEXP wkt, SEXP xptr);
extern "C" SEXP _wk_wk_cpp_handle_wkt(SEXP wkt, SEXP xptr) {
  BEGIN_CPP11
    return cpp11::as_sexp(wk_cpp_handle_wkt(cpp11::as_cpp<cpp11::decay_t<SEXP>>(wkt), cpp11::as_cpp<cpp11::decay_t<SEXP>>(xptr)));
  END_CPP11
}
// wkt-writer.cpp
cpp11::sexp wk_cpp_wkt_writer(int precision, bool trim);
extern "C" SEXP _wk_wk_cpp_wkt_writer(SEXP precision, SEXP trim) {
  BEGIN_CPP11
    return cpp11::as_sexp(wk_cpp_wkt_writer(cpp11::as_cpp<cpp11::decay_t<int>>(precision), cpp11::as_cpp<cpp11::decay_t<bool>>(trim)));
  END_CPP11
}

extern "C" {
/* .Call calls */
extern SEXP _wk_cpp_format_wkb(SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_format_wksxp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_format_wkt(SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_problems_wkb(SEXP);
extern SEXP _wk_cpp_problems_wksxp(SEXP);
extern SEXP _wk_cpp_problems_wkt(SEXP);
extern SEXP _wk_cpp_translate_rct_wkb(SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_translate_rct_wksxp(SEXP);
extern SEXP _wk_cpp_translate_rct_wkt(SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_translate_wkb_xyzm(SEXP);
extern SEXP _wk_cpp_translate_wksxp_xyzm(SEXP);
extern SEXP _wk_cpp_translate_wkt_xyzm(SEXP);
extern SEXP _wk_cpp_translate_xyzm_wkb(SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_translate_xyzm_wksxp(SEXP);
extern SEXP _wk_cpp_translate_xyzm_wkt(SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wkb_translate_wkb(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wkb_translate_wksxp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wkb_translate_wkt(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wksxp_translate_wkb(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wksxp_translate_wksxp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wksxp_translate_wkt(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wkt_translate_wkb(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wkt_translate_wksxp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_cpp_wkt_translate_wkt(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _wk_wk_cpp_handle_wkt(SEXP, SEXP);
extern SEXP _wk_wk_cpp_wkt_writer(SEXP, SEXP);
extern SEXP wk_c_handler_addr();
extern SEXP wk_c_handler_debug_new();
extern SEXP wk_c_handler_problems_new();
extern SEXP wk_c_handler_void_new();
extern SEXP wk_c_read_wkb(SEXP, SEXP);
extern SEXP wk_c_read_xy(SEXP, SEXP);
extern SEXP wk_c_wkb_writer_new();

static const R_CallMethodDef CallEntries[] = {
    {"_wk_cpp_format_wkb",            (DL_FUNC) &_wk_cpp_format_wkb,            4},
    {"_wk_cpp_format_wksxp",          (DL_FUNC) &_wk_cpp_format_wksxp,          4},
    {"_wk_cpp_format_wkt",            (DL_FUNC) &_wk_cpp_format_wkt,            4},
    {"_wk_cpp_problems_wkb",          (DL_FUNC) &_wk_cpp_problems_wkb,          1},
    {"_wk_cpp_problems_wksxp",        (DL_FUNC) &_wk_cpp_problems_wksxp,        1},
    {"_wk_cpp_problems_wkt",          (DL_FUNC) &_wk_cpp_problems_wkt,          1},
    {"_wk_cpp_translate_rct_wkb",     (DL_FUNC) &_wk_cpp_translate_rct_wkb,     3},
    {"_wk_cpp_translate_rct_wksxp",   (DL_FUNC) &_wk_cpp_translate_rct_wksxp,   1},
    {"_wk_cpp_translate_rct_wkt",     (DL_FUNC) &_wk_cpp_translate_rct_wkt,     3},
    {"_wk_cpp_translate_wkb_xyzm",    (DL_FUNC) &_wk_cpp_translate_wkb_xyzm,    1},
    {"_wk_cpp_translate_wksxp_xyzm",  (DL_FUNC) &_wk_cpp_translate_wksxp_xyzm,  1},
    {"_wk_cpp_translate_wkt_xyzm",    (DL_FUNC) &_wk_cpp_translate_wkt_xyzm,    1},
    {"_wk_cpp_translate_xyzm_wkb",    (DL_FUNC) &_wk_cpp_translate_xyzm_wkb,    3},
    {"_wk_cpp_translate_xyzm_wksxp",  (DL_FUNC) &_wk_cpp_translate_xyzm_wksxp,  1},
    {"_wk_cpp_translate_xyzm_wkt",    (DL_FUNC) &_wk_cpp_translate_xyzm_wkt,    3},
    {"_wk_cpp_wkb_translate_wkb",     (DL_FUNC) &_wk_cpp_wkb_translate_wkb,     6},
    {"_wk_cpp_wkb_translate_wksxp",   (DL_FUNC) &_wk_cpp_wkb_translate_wksxp,   4},
    {"_wk_cpp_wkb_translate_wkt",     (DL_FUNC) &_wk_cpp_wkb_translate_wkt,     6},
    {"_wk_cpp_wksxp_translate_wkb",   (DL_FUNC) &_wk_cpp_wksxp_translate_wkb,   6},
    {"_wk_cpp_wksxp_translate_wksxp", (DL_FUNC) &_wk_cpp_wksxp_translate_wksxp, 4},
    {"_wk_cpp_wksxp_translate_wkt",   (DL_FUNC) &_wk_cpp_wksxp_translate_wkt,   6},
    {"_wk_cpp_wkt_translate_wkb",     (DL_FUNC) &_wk_cpp_wkt_translate_wkb,     6},
    {"_wk_cpp_wkt_translate_wksxp",   (DL_FUNC) &_wk_cpp_wkt_translate_wksxp,   4},
    {"_wk_cpp_wkt_translate_wkt",     (DL_FUNC) &_wk_cpp_wkt_translate_wkt,     6},
    {"_wk_wk_cpp_handle_wkt",         (DL_FUNC) &_wk_wk_cpp_handle_wkt,         2},
    {"_wk_wk_cpp_wkt_writer",         (DL_FUNC) &_wk_wk_cpp_wkt_writer,         2},
    {"wk_c_handler_addr",             (DL_FUNC) &wk_c_handler_addr,             0},
    {"wk_c_handler_debug_new",        (DL_FUNC) &wk_c_handler_debug_new,        0},
    {"wk_c_handler_problems_new",     (DL_FUNC) &wk_c_handler_problems_new,     0},
    {"wk_c_handler_void_new",         (DL_FUNC) &wk_c_handler_void_new,         0},
    {"wk_c_read_wkb",                 (DL_FUNC) &wk_c_read_wkb,                 2},
    {"wk_c_read_xy",                  (DL_FUNC) &wk_c_read_xy,                  2},
    {"wk_c_wkb_writer_new",           (DL_FUNC) &wk_c_wkb_writer_new,           0},
    {NULL, NULL, 0}
};
}

extern "C" void R_init_wk(DllInfo* dll){
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
