cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.103.0") # handled by release scripts
set(KF_DEP_VERSION "5.103.0") # handled by release scripts
project(KPty VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.103.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Includes
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(KDEGitCommitHooks)
include(ECMMarkNonGuiExecutable)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMAddQch)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

set(REQUIRED_QT_VERSION 5.15.2)
find_package(Qt${QT_MAJOR_VERSION} "${REQUIRED_QT_VERSION}" CONFIG REQUIRED Core)

find_package(KF5CoreAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5I18n ${KF_DEP_VERSION} REQUIRED)

if (UNIX)
  find_package(UTEMPTER)
  set_package_properties(UTEMPTER PROPERTIES
      TYPE OPTIONAL
      PURPOSE "Required by KPty for managing UTMP entries"
)

  set(HAVE_UTEMPTER ${UTEMPTER_FOUND})
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Pty")

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(
  PROJECT
  VARIABLE_PREFIX KPTY
  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpty_version.h"
  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PtyConfigVersion.cmake"
  SOVERSION 5)

add_definitions(-DTRANSLATION_DOMAIN=\"kpty5\")
ki18n_install(po)

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF 5.97
)

add_subdirectory( src )
if (BUILD_TESTING)
    add_subdirectory( autotests )
endif()

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5Pty_QCH
        FILE KF5PtyQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PtyQchTargets.cmake\")")
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5PtyConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5PtyConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5PtyConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5PtyConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5PtyTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5PtyTargets.cmake NAMESPACE KF5:: )

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/kpty_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPty COMPONENT Devel
)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
