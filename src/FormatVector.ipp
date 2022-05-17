// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "fmt/format.h"

namespace Dca {
using namespace std;
//-----------------------------------------------------------------------------
/*! \brief Format vector `vec` with format `fmt`
  \tparam T      element type of vector, usually automatically inferred
  \param fmt     format string used for vector element
  \param vec     vector to be converted
  \param maxfmt  maximal number of elements to be formatted
                 - if ==0 all elements will be shown
                 - if >0 at most `maxfmt` shown, followed by ",..."
                 - if <0 `maxfmt/2` elements from begin and end shown
  \returns string with formatted vector elements like
           - "[1,2,3]" if `maxfmt==0` or `maxfmt` larger `vec.size()`
           - "[1,2,3,...]" if `maxfmt>0` and vector larger
           - "[1,2,...,8,9]" if `maxfmt<0` and vector larger
 */

template <typename T>
inline string FmtVec(const string& fmt, vector<T> vec, int maxfmt) {
  string res = "[";
  int len = int(vec.size());
  int maxfmt2 = -maxfmt/2;
  for (int i=0; i<len; i++) {
    if (maxfmt > 0 && i >= maxfmt) {
      res += ",...";
      break;
    }
    if (maxfmt < 0) {
      if (i >= maxfmt2 && i < len-maxfmt2) {
        if (i == maxfmt2)  res += ",...";
        continue;
      }
    }    
    if (i > 0) res += ",";
    res += fmt::format(fmt, vec[size_t(i)]);
  }
  res += "]";
  return res;
}


} // end namespace Dca
