cmake_minimum_required (VERSION 3.8)

project ("rtmp")

set(SOURCES
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/chunk_stream.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_client_session.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_control_handler.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_handshake.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_server.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_server_session.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_session_base.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_writer.cpp

    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/byte_stream.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/data_buffer.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/logger.cpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/av/gop_cache.cpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/av/media_stream_manager.cpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/flv_decoder.cpp
)

set(INCLUDES
    src/${SOLUTION_NAME}/${PROJECT_NAME}/format/flv/flv_pub.hpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/net_pub.hpp

    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/chunk_stream.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_client_session.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_control_handler.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_handshake.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_pub.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_request.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_server.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_server_session.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_session_base.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/rtmp_writer.hpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/rtmp/amf/amf0.hpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/tcp/tcp_client.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/tcp/tcp_server.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/net/tcp/tcp_session.hpp

    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/byte_stream.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/data_buffer.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/logger.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/session_aliver.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/stringex.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/timeex.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/timer.hpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/av/gop_cache.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/av/media_packet.hpp
    src/${SOLUTION_NAME}/${PROJECT_NAME}/utils/av/media_stream_manager.hpp
    
    src/${SOLUTION_NAME}/${PROJECT_NAME}/flv_decoder.hpp
)

# Packages
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
##


# Target
add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)
add_library(${SOLUTION_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  IMPORTED_LOCATION ${CMAKE_INSTALL_BINDIR}
  IMPORTED_IMPLIB ${CMAKE_INSTALL_LIBDIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Boost::system
    DDS::core
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src
    ${OPENSSL_INCLUDE_DIR}
)
##

# Install
install(DIRECTORY
    src/${SOLUTION_NAME}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PATTERN "*.hpp"
)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
##

# Export
install(EXPORT ${PROJECT_NAME}Targets
   FILE ${PROJECT_NAME}Config.cmake
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
##

