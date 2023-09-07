#----------------------------------- 1. 경로 파싱#
cmake_minimum_required(VERSION 3.8)

# Project 이름 설정
project(executer)

message("--------------------- ${PROJECT_NAME} CMAKE 시작")

# CMake 버전 설정
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)

find_package(mongocxx REQUIRED)
if(${mongocxx_FOUND})
  message("${PROJECT_NAME}: mongocxx 탐색 성공")
endif()

find_package(bsoncxx REQUIRED)
if(${bsoncxx_FOUND})
  message("${PROJECT_NAME}: bsoncxx 탐색 성공")
endif()

find_package(db REQUIRED)
if(${bsoncxx_FOUND})
  message("${PROJECT_NAME}: db 탐색 성공")
endif()


#----------------------------------- 2. 경로 탐색#
## 1. 해당 프로잭트 헤더 경로 잡기
file(GLOB_RECURSE HDRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)
foreach(HDR ${HDRS})
  message("[${PROJECT_NAME}] 헤더 발견: ${HDR}")
endforeach()

## 2. 소스파일 경로 잡기
file(GLOB_RECURSE SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
)
foreach(SRC ${SRCS})
  message("[${PROJECT_NAME}] 소스 발견: ${SRC}")
endforeach()

#----------------------------------- 3. 빌드# 
## 1. EXE 파일 빌드
add_executable(${PROJECT_NAME}
  ${SRCS}
  ${HDRS}
)

#----------------------------------- 4. 해더 경로 설정#
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#----------------------------------- 5. 라이브러리 링크# 
include_directories(include)

# ament_target_dependencies(
#   robot_db
# )

## 2. 외부 라이브러리 링크 잡기
##=> package_name::exported_library_name
##=> https://answers.ros.org/question/357633/how-to-create-a-ros2-library/
target_link_libraries(${PROJECT_NAME} PRIVATE db::db)
target_link_libraries(${PROJECT_NAME} PRIVATE mongo::bsoncxx_shared)
target_link_libraries(${PROJECT_NAME} PRIVATE mongo::mongocxx_shared)

#----------------------------------- 6. 빌드 후처리# 
## 2. 빌드 경로 구성
# install(TARGETS ${PROJECT_NAME}
#   EXPORT "export_${PROJECT_NAME}"
#   ARCHIVE DESTINATION lib
#   LIBRARY DESTINATION lib
#   RUNTIME DESTINATION lib
#   INCLUDES DESTINATION include/${PROJECT_NAME}
# )

##=> ros2으로 실행 가능하게 해줌
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

message("--------------------- ${PACKAGE_NAME} CMAKE 종료")

ament_package()