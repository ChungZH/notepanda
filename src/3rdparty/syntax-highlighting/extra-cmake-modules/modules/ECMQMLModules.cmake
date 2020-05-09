#.rst:
# ECMQMLModules
# -------------
#
# Find QML import modules through a find_qmlmodule() call.
# It uses the qmlplugindump application to find the plugins and sets them up as
# runtime dependencies.
#
# This is useful so that when we configure a project we are noified when some
# QML imports are not present in the system, thus having the application compilable
# but fail at runtime.
#
# ::
#
#   ecm_find_qmlmodule(<module_name> <version>...)
#
# Any further arguments passed will be forwarded into a find_package() call. See
# find_package() documentation for more information.
#
# Usage example:
#
# .. code-block:: cmake
#
#   ecm_find_qmlmodule(org.kde.kirigami 2.1)
#
# Since 5.38.0.
#
#=============================================================================
# Copyright 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
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

set(MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})

function(ecm_find_qmlmodule MODULE_NAME VERSION)
    set(GENMODULE "${MODULE_NAME}-QMLModule")

    configure_file("${MODULES_DIR}/ECMFindQMLModule.cmake.in" "Find${GENMODULE}.cmake" @ONLY)

    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}" ${CMAKE_MODULE_PATH})
    find_package(${GENMODULE} ${ARGN})

    if(COMMAND set_package_properties)
        set_package_properties(${GENMODULE} PROPERTIES
            DESCRIPTION "QML module '${MODULE_NAME}' is a runtime dependency."
            TYPE RUNTIME)
    endif()
endfunction()
