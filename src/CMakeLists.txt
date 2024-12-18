add_library(KF6Pty)
add_library(KF6::Pty ALIAS KF6Pty)

set_target_properties(KF6Pty PROPERTIES
   VERSION     ${KPTY_VERSION}
   SOVERSION   ${KPTY_SOVERSION}
   EXPORT_NAME Pty
)

target_sources(KF6Pty PRIVATE
    kpty.cpp
    kptydevice.cpp
    kptydevice.h
    kpty.h
    kpty_p.h
    kptyprocess.cpp
    kptyprocess.h
)

ecm_generate_export_header(KF6Pty
    BASE_NAME KPty
    GROUP_BASE_NAME KF
    VERSION ${KPTY_VERSION}
    USE_VERSION_HEADER
    DEPRECATED_BASE_VERSION 0
)

ecm_qt_declare_logging_category(KF6Pty
    HEADER kpty_debug.h
    IDENTIFIER KPTY_LOG
    CATEGORY_NAME kf.pty
    DESCRIPTION "KPty"
    EXPORT KPTY
)

include(ConfigureChecks.cmake)
configure_file(config-pty.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-pty.h )

target_include_directories(KF6Pty INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KPty>")

target_link_libraries(KF6Pty PUBLIC Qt6::Core
                                    KF6::CoreAddons # KProcess
                             PRIVATE
                                    ${UTIL_LIBRARY}
                                    KF6::I18n)

target_include_directories(KF6Pty PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/..)
if(UTEMPTER_FOUND)
  target_compile_definitions(KF6Pty PRIVATE ${UTEMPTER_COMPILE_FLAGS})
endif()

ecm_generate_headers(KPty_HEADERS
  HEADER_NAMES
  KPty
  KPtyDevice
  KPtyProcess

  REQUIRED_HEADERS KPty_HEADERS
)

install(TARGETS KF6Pty EXPORT KF6PtyTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kpty_export.h
  ${KPty_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPty COMPONENT Devel
)

ecm_generate_qdoc(KF6Pty kpty.qdocconf)

########### next target ###############

if (NOT HAVE_OPENPTY)
   add_executable(kgrantpty kgrantpty.c)
   ecm_mark_nongui_executable(kgrantpty)
   install(TARGETS kgrantpty DESTINATION ${KDE_INSTALL_LIBEXECDIR_KF})
endif ()

ecm_qt_install_logging_categories(
    EXPORT KPTY
    FILE kpty.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)
