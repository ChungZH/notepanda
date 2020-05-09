#.rst:
# FindSphinx
# ----------
#
# Try to find the Sphinx documentation builder.
#
# This will define the following variables:
#
# ``Sphinx_FOUND``
#     True if (the requested version of) Sphinx is available
# ``Sphinx_VERSION``
#     The version of the Sphinx documentation builder.
# ``Sphinx_BUILD_EXECUTABLE``
#     The path to the Sphinx documentation builder executable.
#
# If ``Sphinx_FOUND`` is TRUE, it will also define the following imported target:
#
# ``Sphinx::Build``
#     The Sphinx documentation builder.
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

# Distros sometimes rename Python executables to allow for parallel
# installation of Python2 and Python3 versions
find_program(Sphinx_BUILD_EXECUTABLE
    NAMES
        sphinx-build
        sphinx-build2
        sphinx-build3
    DOC "Sphinx Documentation Builder (https://www.sphinx-doc.org/)"
)

if (Sphinx_BUILD_EXECUTABLE)
    if(NOT TARGET Sphinx::Build)
        add_executable(Sphinx::Build IMPORTED)
        set_target_properties(Sphinx::Build PROPERTIES
            IMPORTED_LOCATION "${Sphinx_BUILD_EXECUTABLE}"
        )
    endif()

    execute_process(
        COMMAND "${Sphinx_BUILD_EXECUTABLE}" --version
        OUTPUT_VARIABLE _Sphinx_version_raw
        ERROR_VARIABLE _Sphinx_version_raw
    )
    if (_Sphinx_version_raw MATCHES "^Sphinx \\([^)]*\\) ([0-9]+(\\.[0-9]+)*)")
        set(Sphinx_VERSION "${CMAKE_MATCH_1}")
    endif()
    unset(_Sphinx_version_raw)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sphinx
    FOUND_VAR
        Sphinx_FOUND
    REQUIRED_VARS
        Sphinx_BUILD_EXECUTABLE
    VERSION_VAR
        Sphinx_VERSION
)

mark_as_advanced(Sphinx_BUILD_EXECUTABLE)
