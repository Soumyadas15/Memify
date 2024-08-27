cmake_minimum_required(VERSION 3.10)
project(Memify)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/server

    ${PROJECT_SOURCE_DIR}/cache
    ${PROJECT_SOURCE_DIR}/cache/r-tree

    ${PROJECT_SOURCE_DIR}/cache/geopoints

    ${PROJECT_SOURCE_DIR}/core

    ${PROJECT_SOURCE_DIR}/utils/logs
    ${PROJECT_SOURCE_DIR}/utils/logs/file
    ${PROJECT_SOURCE_DIR}/utils/logs/console
    ${PROJECT_SOURCE_DIR}/utils/logs/console
    ${PROJECT_SOURCE_DIR}/utils/logs/manager

    ${PROJECT_SOURCE_DIR}/utils/parser

    ${PROJECT_SOURCE_DIR}/config

    ${PROJECT_SOURCE_DIR}/persistence/AOF
    ${PROJECT_SOURCE_DIR}/persistence/Snapshot

    ${PROJECT_SOURCE_DIR}/security
    ${PROJECT_SOURCE_DIR}/connection
    ${PROJECT_SOURCE_DIR}/connection/message
    ${PROJECT_SOURCE_DIR}/connection/response
    ${PROJECT_SOURCE_DIR}/connection/message/parser

    ${OPENSSL_INCLUDE_DIR}
)

add_executable(Memify
    src/main.cpp
    server/Server.cpp

    cache/Cache.cpp
    cache/CacheSet.cpp
    cache/CacheGet.cpp
    cache/CacheDelete.cpp
    cache/CacheCleanup.cpp
    cache/CacheMoveToFront.cpp
    cache/CacheEvict.cpp

    cache/geopoints/SetGeoPoint.cpp
    cache/geopoints/GetGeoPoint.cpp

    persistence/snapshot/Snapshot.cpp
    persistence/AOF/AOF.cpp

    config/INIReader.cpp
    config/ini.cpp
    security/HMACUtil.cpp

    connection/ConnectionHandler.cpp
    connection/HandleClient.cpp
    connection/ProcessMessage.cpp
    connection/SendResponse.cpp
    connection/VerifySignature.cpp

    connection/message/MessageProcessor.cpp
    connection/message/handlers/HandleErrors.cpp
    connection/message/handlers/HandleErrors.cpp
    connection/message/handlers/HandleGet.cpp
    connection/message/handlers/HandleSet.cpp
    connection/message/handlers/HandlePing.cpp
    connection/message/handlers/geolocation/HandleGeoSet.cpp
    connection/message/handlers/geolocation/HandleGeoGet.cpp
    connection/message/handlers/geolocation/HandleGeoRadius.cpp

    connection/response/ResponseSender.cpp

    utils/logs/file/FileLogger.cpp
    utils/logs/console/ConsoleLogger.cpp
    utils/logs/manager/LoggerManager.cpp

    utils/parser/CommandParser.cpp
    utils/parser/Serializer.cpp
    utils/parser/parsing/Error.cpp
    utils/parser/parsing/ParseSimpleString.cpp
    utils/parser/parsing/ParseBulkString.cpp
    utils/parser/parsing/ParsseArray.cpp
    utils/parser/parsing/ParseInteger.cpp
    utils/parser/parsing/ParseFloat.cpp
)

target_link_libraries(Memify pthread ${OPENSSL_LIBRARIES})