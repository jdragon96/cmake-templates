#ifndef UTILLIB_STRINGUTIL_H
#define UTILLIB_STRINGUTIL_H

#include <string>

class StringUtil {
public:
  //! 두 문자를 합친다.
  static std::string AddString(std::string s1, std::string s2);
};

#endif UTILLIB_STRINGUTIL_H