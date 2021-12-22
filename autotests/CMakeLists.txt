include(ECMMarkAsTest)
include(ECMMarkNonGuiExecutable)

find_package(Qt${QT_MAJOR_VERSION} "${REQUIRED_QT_VERSION}" CONFIG REQUIRED Test)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

add_executable(kptyprocesstest kptyprocesstest.cpp)
target_link_libraries(kptyprocesstest KF5::Pty Qt${QT_MAJOR_VERSION}::Test)
ecm_mark_as_test(kptyprocesstest)
ecm_mark_nongui_executable(kptyprocesstest)
add_test(NAME kptyprocesstest COMMAND kptyprocesstest)
