#include <iostream>
#include <string>
#include <UtilLib/string_utils/string_util.h>

int main(){

  std::cout << StringUtil::AddString("문자1", " 추가된 문자") << std::endl;
  std::cout << StringUtil::AddString("Why", " Broken this string?") << std::endl;

  return 0;
}