find_package(PkgConfig)
pkg_check_modules(GTK gtk+-2.0)
include_directories(${GTK_INCLUDE_DIRS})
#find_package(GTK 2.6 REQUIRED gtk)
#find_package(GTK2 2.6 REQUIRED gtk)
#include_directories(${GTK_INCLUDE_DIRS})
#include_directories(${GTK2_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${MaTest_SOURCE_DIR}/src/logics)
SET(SRC
    MaTest.c
    callbacks.c
    evaluation.c
    user.c
    user_gui.c
    user_text.c)
SET(LIBSRC
    logics
    ${GTK_LIBRARIES})
#    ${GTK2_LIBRARIES})
#SET(CMAKE_C_FLAGS "${GTK_CFLAGS} ${GTHREAD_CFLAGS}")
#STRING(REGEX REPLACE "-l" "" GTK_LIBRARIES "${GTK_LINK_FLAGS} ${GTHREAD_LINK_FLAGS}")
ADD_EXECUTABLE(matest ${SRC})
TARGET_LINK_LIBRARIES(matest ${LIBSRC})
add_subdirectory (logics)
