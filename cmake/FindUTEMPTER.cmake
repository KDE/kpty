# - Try to find the UTEMPTER directory notification library
# Once done this will define
#
#  UTEMPTER_FOUND - system has UTEMPTER
#  UTEMPTER_INCLUDE_DIR - the UTEMPTER include directory
#  UTEMPTER_LIBRARIES - The libraries needed to use UTEMPTER

# Copyright (c) 2015, Hrvoje Senjan, <hrvoje.senjan@gmail.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.


find_program (UTEMPTER_EXECUTABLE utempter PATHS
    ${KDE_INSTALL_FULL_LIBEXECDIR}/utempter
    ${KDE_INSTALL_FULL_LIBDIR}/utempter
    /usr/libexec/utempter
)

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

