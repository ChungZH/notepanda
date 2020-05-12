#.rst:
# FindIcoTool
# -----------
#
# Try to find icotool.
#
# If the icotool executable is not in your PATH, you can provide
# an alternative name or full path location with the ``IcoTool_EXECUTABLE``
# variable.
#
# This will define the following variables:
#
# ``IcoTool_FOUND``
#     True if icotool is available.
#
# ``IcoTool_EXECUTABLE``
#     The icotool executable.
#
# If ``IcoTool_FOUND`` is TRUE, it will also define the following imported
# target:
#
# ``IcoTool::IcoTool``
#     The icotool executable.
#
# Since 5.49.

#=============================================================================
# Copyright 2017 Vincent Pinon <vpinon@kde.org>
# Copyright 2014 Alex Merry <alex.merry@kde.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

include(${CMAKE_CURRENT_LIST_DIR}/ECMFindModuleHelpersStub.cmake)
ecm_find_package_version_check(IcoTool)
find_program(IcoTool_EXECUTABLE NAMES icotool)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IcoTool
    FOUND_VAR
        IcoTool_FOUND
    REQUIRED_VARS
        IcoTool_EXECUTABLE
)
mark_as_advanced(IcoTool_EXECUTABLE)

if (IcoTool_FOUND)
    if (NOT TARGET IcoTool::IcoTool)
        add_executable(IcoTool::IcoTool IMPORTED)
        set_target_properties(IcoTool::IcoTool PROPERTIES
            IMPORTED_LOCATION "${IcoTool_EXECUTABLE}"
        )
    endif()
endif()

include(FeatureSummary)
set_package_properties(IcoTool PROPERTIES
    URL "https://www.nongnu.org/icoutils/"
    DESCRIPTION "Executable that converts a collection of PNG files into a Windows icon file"
)
