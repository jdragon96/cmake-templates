#include "UtilLib/string_utils/string_util.h"
#include <string>

std::string StringUtil::AddString(std::string s1, std::string s2) {
  std::string s("");
  s += s1;
  s += s2;
  return s;
}