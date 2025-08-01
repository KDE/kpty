cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "6.18.0") # handled by release scripts
set(KF_DEP_VERSION "6.17.0") # handled by release scripts
project(KPty VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 6.17.0  NO_MODULE)
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
include(ECMGenerateExportHeader)
include(ECMGenerateQDoc)
include(CMakePackageConfigHelpers)

set(REQUIRED_QT_VERSION 6.7.0)
find_package(Qt6 "${REQUIRED_QT_VERSION}" CONFIG REQUIRED Core)

find_package(KF6CoreAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF6I18n ${KF_DEP_VERSION} REQUIRED)

if (UNIX)
  find_package(UTEMPTER)
  set_package_properties(UTEMPTER PROPERTIES
      TYPE OPTIONAL
      PURPOSE "Required by KPty for managing UTMP entries"
)

  set(HAVE_UTEMPTER ${UTEMPTER_FOUND})
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF6Pty")

set(kpty_version_header "${CMAKE_CURRENT_BINARY_DIR}/src/kpty_version.h")
ecm_setup_version(
  PROJECT
  VARIABLE_PREFIX KPTY
  VERSION_HEADER "${kpty_version_header}"
  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF6PtyConfigVersion.cmake"
  SOVERSION 6)

add_definitions(-DTRANSLATION_DOMAIN=\"kpty6\")
ki18n_install(po)

ecm_set_disabled_deprecation_versions(
    QT 6.10.0
    KF 6.16.0
)

add_subdirectory( src )
if (BUILD_TESTING)
    add_subdirectory( autotests )
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF6PtyConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF6PtyConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF6PtyConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF6PtyConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF6PtyTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF6PtyTargets.cmake NAMESPACE KF6:: )

install(
    FILES ${kpty_version_header}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPty
    COMPONENT Devel
)

include(ECMFeatureSummary)
ecm_feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
