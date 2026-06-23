cmake_minimum_required(VERSION 3.16...3.30)
project(PCB_Background_Sim)

# 1. Geant4 패키지 찾기
find_package(Geant4 REQUIRED ui_all vis_all)

# 2. Geant4 설정 반영
include(${Geant4_USE_FILE})

# 3. 소스 파일 등록
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/main.cc
    ${PROJECT_SOURCE_DIR}/src/*.cc
)

# 4. 실행 파일 생성
add_executable(PCB_Background_Sim ${SOURCES})

# 5. 헤더 파일 경로 지정 (타겟에 직접 바인딩하여 컴파일러가 확실히 인식하도록 수정)
target_include_directories(PCB_Background_Sim PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${Geant4_INCLUDE_DIRS}
)

# 6. 라이브러리 링크
target_link_libraries(PCB_Background_Sim PRIVATE ${Geant4_LIBRARIES})