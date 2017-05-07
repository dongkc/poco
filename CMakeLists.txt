cmake_minimum_required(VERSION 2.8)

SET(CMAKE_SYSTEM_NAME Linux)

LINK_DIRECTORIES(${CMAKE_FIND_ROOT_PATH}/usr/lib)
INCLUDE_DIRECTORIES(${CMAKE_FIND_ROOT_PATH}/usr/include)

PROJECT(LIBMODBUS)

SET(CMAKE_CXX_FLAGS "-Wall -O2 -fmessage-length=0 -fno-strict-aliasing --std=c++0x -Wno-psabi -DQ_EVT_CTOR")
SET(CMAKE_C_FLAGS   "-Wall -O2 -fmessage-length=0 -fno-strict-aliasing -Wno-psabi -DQ_EVT_CTOR")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -ltcmalloc")
SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)

#AUX_SOURCE_DIRECTORY(. RTU_SRC)
#ADD_EXECUTABLE(gateway ${RTU_SRC})

ADD_EXECUTABLE(gateway main.cpp)
TARGET_LINK_LIBRARIES(gateway
                      glog
                      PocoUtil
                      PocoXML
                      PocoNet
                      PocoFoundation)

ADD_EXECUTABLE(xml xml.cpp)
TARGET_LINK_LIBRARIES(xml
                      glog
                      PocoUtil
                      PocoXML
                      PocoNet
                      PocoFoundation)

ADD_EXECUTABLE(service service.cpp)
TARGET_LINK_LIBRARIES(service
                      glog
                      PocoUtil
                      PocoXML
                      PocoNet
                      PocoFoundation)
