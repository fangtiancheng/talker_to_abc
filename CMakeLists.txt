cmake_minimum_required(VERSION 3.13)
project(talker_to_abc)
set(CMAKE_CXX_STANDARD 17)

find_package(Alembic REQUIRED CONFIG)
include_directories(${Alembic_INCLUDE_DIRS})

add_subdirectory(3rdparty)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC cnpy happly Alembic::Alembic)
