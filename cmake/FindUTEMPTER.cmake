# - Try to find the UTEMPTER directory notification library
# Once done this will define
#
#  UTEMPTER_FOUND - system has UTEMPTER
#  UTEMPTER_INCLUDE_DIR - the UTEMPTER include directory
#  UTEMPTER_LIBRARIES - The libraries needed to use UTEMPTER

# SPDX-FileCopyrightText: 2015 Hrvoje Senjan <hrvoje.senjan@gmail.org>
#
# SPDX-License-Identifier: BSD-3-Clause


# use find_file instead of find_program until this is fixed:
# https://gitlab.kitware.com/cmake/cmake/issues/10468
find_file (UTEMPTER_EXECUTABLE utempter PATHS
    ${KDE_INSTALL_FULL_LIBEXECDIR}/utempter
    ${KDE_INSTALL_FULL_LIBDIR}/utempter
    ${CMAKE_INSTALL_PREFIX}/libexec/utempter
    ${CMAKE_INSTALL_PREFIX}/lib/utempter
    /usr/libexec/utempter
    /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/utempter
    /usr/lib/utempter
)

# On FreeBSD for example we have to use ulog-helper
if (NOT UTEMPTER_EXECUTABLE)
    find_program (UTEMPTER_EXECUTABLE ulog-helper PATHS
        /usr/libexec
    )
    if (UTEMPTER_EXECUTABLE)
        add_definitions(-DUTEMPTER_ULOG=1)
    endif ()
endif ()

if (UTEMPTER_EXECUTABLE)
    add_definitions(-DUTEMPTER_PATH=\"${UTEMPTER_EXECUTABLE}\")
endif ()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (UTEMPTER DEFAULT_MSG UTEMPTER_EXECUTABLE)


set_package_properties (UTEMPTER PROPERTIES
    URL "ftp://ftp.altlinux.org/pub/people/ldv/utempter/"
    DESCRIPTION "Allows non-privileged applications such as terminal emulators to modify the utmp database without having to be setuid root."
)

mark_as_advanced (UTEMPTER_EXECUTABLE)

