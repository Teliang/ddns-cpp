cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ddns-cpp)

# Set the C++ standard to C++17 (or whatever version you need)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB DDNS_CPPFILES src/*.cpp)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add the source files
add_executable(${PROJECT_NAME} ${DDNS_CPPFILES})

# Optional: Include directories if you have headers
target_include_directories(${PROJECT_NAME} PRIVATE ./lib)

# Optional: Add libraries if needed
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)
install(CODE 
    "IF(NOT EXISTS /etc/${PROJECT_NAME}.json)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/etc/${PROJECT_NAME}.json DESTINATION /etc)
    ENDIF()")

install(CODE 
    "IF(NOT EXISTS /usr/lib/systemd/system/${PROJECT_NAME}.service)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/etc/${PROJECT_NAME}.service DESTINATION /usr/lib/systemd/system)
    ENDIF()")
