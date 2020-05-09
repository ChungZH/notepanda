#.rst:
# FindQCollectionGenerator
# ------------------------
#
# Try to find the Qt help collection generator.
#
# This will define the following variables:
#
# ``QCollectionGenerator_FOUND``
#     True if (the requested version of) Sphinx is available
# ``QCollectionGenerator_VERSION``
#     The version of the Qt help collection generator. Note that this not the
#     same as the version of Qt it is provided by.
# ``QCollectionGenerator_QT_VERSION``
#     The version of Qt that the Qt help collection generator is from.
# ``QCollectionGenerator_EXECUTABLE``
#     The path to the Qt help collection generator executable.
#
# If ``QCollectionGenerator_FOUND`` is TRUE, it will also define the following
# imported target:
#
# ``QCollectionGenerator::Generator``
#     The Qt help collection generator.
#
# In general we recommend using the imported target, as it is easier to use.
#
# Since 5.17.0.

#=============================================================================
# Copyright 2015 Alex Merry <alex.merry@kde.org>
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

find_program(QCollectionGenerator_EXECUTABLE
    NAMES
        qcollectiongenerator
        qcollectiongenerator-qt4
        qcollectiongenerator-qt5
    DOC "Qt help collection generator"
)

if (QCollectionGenerator_EXECUTABLE)
    if(NOT TARGET QCollectionGenerator::Generator)
        add_executable(QCollectionGenerator::Generator IMPORTED)
        set_target_properties(QCollectionGenerator::Generator PROPERTIES
            IMPORTED_LOCATION "${QCollectionGenerator_EXECUTABLE}"
        )
    endif()

    execute_process(
        COMMAND "${QCollectionGenerator_EXECUTABLE}" -v
        OUTPUT_VARIABLE _QCollectionGenerator_version_raw
        ERROR_VARIABLE _QCollectionGenerator_version_raw
    )
    if (_QCollectionGenerator_version_raw MATCHES "^Qt Collection Generator version ([0-9]+(\\.[0-9]+)*) \\(Qt ([0-9]+(\\.[0-9]+)*)\\)")
        set(QCollectionGenerator_VERSION "${CMAKE_MATCH_1}")
        set(QCollectionGenerator_QT_VERSION "${CMAKE_MATCH_3}")
    endif()
    unset(_QCollectionGenerator_version_raw)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QCollectionGenerator
    FOUND_VAR
        QCollectionGenerator_FOUND
    REQUIRED_VARS
        QCollectionGenerator_EXECUTABLE
    VERSION_VAR
        QCollectionGenerator_VERSION
)

mark_as_advanced(QCollectionGenerator_EXECUTABLE)
