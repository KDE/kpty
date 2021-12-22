add_library(KF5Pty)
add_library(KF5::Pty ALIAS KF5Pty)

target_sources(KF5Pty PRIVATE
    kpty.cpp
    kptydevice.cpp
    kptydevice.h
    kpty.h
    kpty_p.h
    kptyprocess.cpp
    kptyprocess.h
)

ecm_qt_declare_logging_category(KF5Pty
    HEADER kpty_debug.h
    IDENTIFIER KPTY_LOG
    CATEGORY_NAME kf.pty
    DESCRIPTION "KPty"
    EXPORT KPTY
)

generate_export_header(KF5Pty BASE_NAME KPty)

include(ConfigureChecks.cmake)
configure_file(config-pty.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-pty.h )

target_include_directories(KF5Pty INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KPty>")

target_link_libraries(KF5Pty PUBLIC Qt${QT_MAJOR_VERSION}::Core
                                    KF5::CoreAddons # KProcess
                             PRIVATE
                                    ${UTIL_LIBRARY}
                                    KF5::I18n)

target_include_directories(KF5Pty PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/..)

set_target_properties(KF5Pty PROPERTIES
   VERSION ${KPTY_VERSION}
   SOVERSION ${KPTY_SOVERSION}
   EXPORT_NAME Pty
)

ecm_generate_headers(KPty_HEADERS
  HEADER_NAMES
  KPty
  KPtyDevice
  KPtyProcess

  REQUIRED_HEADERS KPty_HEADERS
)

install(TARGETS KF5Pty EXPORT KF5PtyTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kpty_export.h
  ${KPty_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPty COMPONENT Devel
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5Pty_QCH
        NAME KPty
        BASE_NAME KF5Pty
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KPty_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
            KF5CoreAddons_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KPTY_EXPORT
            KPTY_DEPRECATED
            KPTY_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

########### next target ###############

if (NOT HAVE_OPENPTY)
   add_executable(kgrantpty kgrantpty.c)
   ecm_mark_nongui_executable(kgrantpty)
   install(TARGETS kgrantpty DESTINATION ${KDE_INSTALL_LIBEXECDIR_KF})
endif ()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KPty LIB_NAME KF5Pty DEPS "core KCoreAddons" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KPty)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

ecm_qt_install_logging_categories(
    EXPORT KPTY
    FILE kpty.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)
