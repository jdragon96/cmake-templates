#! library template v1.0.0
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

# 1-4 환경변수 설정
set(SUBLIB_DIR "lib")
set(OUTPUT_DIR "bin")

message("---------- ${PROJECT_NAME} CMAKE 시작 ------------")

#================================= 프로젝트 의존성 탐색 #
# 2-1 상용 패키지 찾기

# 2-2 std 패키지 찾기
# target_link_libraries(${PROJECT_NAME} PRIVATE string)

# 2-3 파일 경로 탐색
# 2-4-1 src 경로 탐색
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
)

# 2-4-2 header 경로 탐색
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# 2-4-3 라이브러리 패키지 뽑아내기
FILE(GLOB SUB_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/${SUBLIB_DIR}/*)

message("[${PROJECT_NAME}] 소스파일 목록: ${SRC_FILES}")
message("[${PROJECT_NAME}] 헤더파일 목록: ${HEADER_FILES}")
message("[${PROJECT_NAME}] 하위 라이브러리 목록: ${SUB_LIBRARIES}")
#================================= 빌드 #
# 3-1 빌드
# 3-1-1 exe 빌드
# add_executable(${PROJECT_NAME}
#   ${SRC_FILES}
# )

# 3-1-2 라이브러리 빌드
## STATIC, SHARED, ...
add_library(${PROJECT_NAME} STATIC
  ${SRC_FILES}
)

# 3-2 Header 경로잡기
# 3-2-1 본인 프로젝트 헤더 경로잡기
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 3-2-2 외부 라이브러리 Header 추가
# 3-2-2-1 subdirecory를 순회하여 외부 라이브러리 체크
foreach(sublib ${SUB_LIBRARIES})
  add_subdirectory(${sublib})
  message("//! 라이브러리 감지: ${sublib}")
endforeach()

# 3-2-2-2 Header 추가
foreach(sublib ${SUB_LIBRARIES})
  get_filename_component(LIB_NAME ${sublib} NAME)
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${sublib}/include
  )
  message("//! 라이브러리 헤더파일 추가: ${LIB_NAME}")
endforeach()

#================================= 출력경로 설정 #
# 5.1 바이너리 출력 경로 설정
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_DIR}"
)

message("[${PROJECT_NAME}] 빌드 타입: ${CMAKE_BUILD_TYPE}")
message("[${PROJECT_NAME}] 바이너리 경로: ${CMAKE_CURRENT_SOURCE_DIR}/bin")
message("[${PROJECT_NAME}] 헤더 경로: ${HEADER_FILES}")
message("[${PROJECT_NAME}] CMAKE 경로: ${CMAKE_CURRENT_SOURCE_DIR}")
message("---------- ${PROJECT_NAME} CMAKE 종료 ------------")