cmake_minimum_required(VERSION 3.2)

if(WIN32 OR WIN64)
    message(STATUS "My Windows detected")
    project(realty VERSION 0.0.1000)
    set(BOOST_ROOT "C:/Boost")
else()
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
endif()

find_package(Threads)

add_executable(rclst main_rclst.cpp realty_rclst.cpp realty_rclst.h common.h)
add_executable(rclss main_rclss.cpp realty_rclss.cpp realty_rclss.h common.h)

if(WIN32 OR WIN64)
   set(DLIB_DIR "C:\\dlib-19.19\\dlib-19.19\\dlib")
   set(DLIB_INCLUDE "C:\\dlib-19.19\\dlib-19.19\\dlib")

   # find_package(dlib)

   #set(dlib_DIR "C:\\dlib-19.19\\dlib-19.19\\dlib")

   target_link_libraries(rclst openblas)
   target_link_libraries(rclst dlib)
   target_link_libraries(rclss openblas)
   target_link_libraries(rclss dlib)


else()
    target_link_libraries(rclst openblas)
    target_link_libraries(rclst dlib)
    target_link_libraries(rclss openblas)
    target_link_libraries(rclss dlib)

endif()

target_link_libraries(rclst
    ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(rclss
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WIN32 OR WIN64)
    set_target_properties(rclst rclss PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED ON
			COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
else()
    set_target_properties(rclst rclss PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED ON
            COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")

    set(CMAKE_BUILD_TYPE Release)
endif()

install(TARGETS rclst rclss RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT "znseday")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

#enable_testing()
#add_test(my_tests_ my_tests)
