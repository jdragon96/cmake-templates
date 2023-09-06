#! template v1.0.0
#================================= 프로젝트 설정 #
# 1-1 CMake 버전 설정
cmake_minimum_required(VERSION 3.11)

# 1-2 프로젝트 메타정보 입력
project(
  UtilLib
  VERSION 1.0.1
  DESCRIPTION "유틸리티 프로젝트"
  LANGUAGES CXX)

# 1-3 CPP 버전 명시
if(NOT CMAKE_CXX_STANDARD)   
set(CMAKE_CXX_STANDARD 17)   
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
endif()

#================================= 빌드 #
# 3-0 파일 경로잡기
# 3-0-1 src 파일 파싱하기
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
)

# 3-0-2 header 파일 파싱하기
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# 3-1 빌드

# 3-1-1 exe 빌드
#=> 
# add_executable(${PROJECT_NAME} SHARED
#   ${SRC_FILES}
# )

# 3-1-2 라이브러리 빌드
#=> https://cmake.org/cmake/help/latest/command/add_library.html
add_library(${PROJECT_NAME} SHARED
  ${SRC_FILES}
)

#================================= 프로젝트 의존성 추가 #
# 4-1 라이브러리 경로 추가
# target_link_libraries(${PROJECT_NAME} PRIVATE string)

# 4-2 Header 경로 추가
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

# 4-3 외부 라이브러리 Header 추가


#================================= 프로젝트 설정 #



