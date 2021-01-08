cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(BUILD_TRANSPORT_CURL "Build an HTTP transport implementation with CURL" OFF)
option(BUILD_TRANSPORT_WINHTTP "Build an HTTP transport implementation with WIN HTTP" OFF)
option(BUILD_TRANSPORT_CUSTOM "Implementation for AzureSdkGetCustomHttpTransport method must be linked to the final application" OFF)

project(storagetoken LANGUAGES CXX)

set(AZ_ALL_LIBRARIES ON)

add_subdirectory(azure-sdk-for-cpp/sdk/core/azure-core)
add_subdirectory(azure-sdk-for-cpp/sdk/identity/azure-identity)
add_subdirectory(azure-sdk-for-cpp/sdk/storage/azure-storage-common)
add_subdirectory(azure-sdk-for-cpp/sdk/storage/azure-storage-blobs)

add_executable(storagetoken main.cpp)

target_link_libraries(storagetoken PRIVATE
    Azure::azure-core
    Azure::azure-identity
    Azure::azure-storage-common
    Azure::azure-storage-blobs)
