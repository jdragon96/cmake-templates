#include <iostream>
#include <db/model/Student.hpp>
#include <db/resource/MongoManager.hpp>

int main() {
  std::cout << "Build successfully" << std::endl;
  using namespace DB::Resource;
  MongoParameter param;
  param.ip = "localhost";
  param.port = 27017;
  param.minPoolSize = 5;
  param.maxPoolSize = 10;

  if (MongoManager::instance().setConnString(param)) {
    std::cout << "파라미터 설정 완료" << std::endl;
  }

  if (MongoManager::instance().build()) {
    std::cout << "MongoDB 커넥션 성공" << std::endl;
  }

  bool connectionFlag;
  auto con = MongoManager::instance().get_connection(connectionFlag);
  if (connectionFlag) {
    std::cout << "MongoDB 커넥션 생성 완료" << std::endl;
  } else {
    std::cout << "MongoDB 커넥션 생성 실패" << std::endl;
    return 0;
  }

  using namespace DB::Model;
  Student s1;
  s1.Name = "철수";
  s1.Address = "서울특별시";
  s1.Age = 24;
  s1.StudentID = "12315412";
  StudentGrade grade1;
  grade1.Class = "컴퓨터공학의 역사";
  grade1.Grade = GradeEnum::A;
  s1.Grades.push_back(grade1);

  StudentGrade grade2;
  grade2.Class = "전지가학";
  grade2.Grade = GradeEnum::A;
  s1.Grades.push_back(grade2);

  std::cout << "입력 시작" << std::endl;
  s1.insertOne(con);
  std::cout << "입력 완료" << std::endl;

  std::cout << "탐색 시작" << std::endl;
  Student s2;
  s2.StudentID = "12315412";
  s2.findOne(con);

  return 0;
}