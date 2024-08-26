cmake_minimum_required(VERSION 3.10)
project(Memify)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}/cache
    ${PROJECT_SOURCE_DIR}/core

    ${PROJECT_SOURCE_DIR}/utils/logs
    ${PROJECT_SOURCE_DIR}/utils/logs/file
    ${PROJECT_SOURCE_DIR}/utils/logs/console
    ${PROJECT_SOURCE_DIR}/utils/logs/manager

    ${PROJECT_SOURCE_DIR}/config

    ${PROJECT_SOURCE_DIR}/persistence/AOF
    ${PROJECT_SOURCE_DIR}/persistence/Snapshot

    ${PROJECT_SOURCE_DIR}/security
    ${PROJECT_SOURCE_DIR}/connection
    
    ${OPENSSL_INCLUDE_DIR}
)

add_executable(Memify
    src/main.cpp
    server/Server.cpp
    cache/Cache.cpp

    persistence/snapshot/Snapshot.cpp
    persistence/AOF/AOF.cpp

    config/INIReader.cpp
    config/ini.cpp
    security/HMACUtil.cpp

    connection/Connectionhandler.cpp
    connection/MessageProcessor.cpp
    connection/ResponseSender.cpp

    utils/logs/file/FileLogger.cpp
    utils/logs/console/ConsoleLogger.cpp
    utils/logs/manager/LoggerManager.cpp
)

target_link_libraries(Memify pthread ${OPENSSL_LIBRARIES})