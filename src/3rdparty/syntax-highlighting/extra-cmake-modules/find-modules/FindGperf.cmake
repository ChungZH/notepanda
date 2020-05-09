#.rst:
# FindGperf
# -----------
#
# Try to find GNU gperf.
#
# If the gperf executable is not in your PATH, you can provide
# an alternative name or full path location with the ``Gperf_EXECUTABLE``
# variable.
#
# This will define the following variables:
#
# ``Gperf_FOUND``
#     True if gperf is available.
#
# ``Gperf_EXECUTABLE``
#     The gperf executable.
#
# If ``Gperf_FOUND`` is TRUE, it will also define the following imported
# target:
#
# ``GPerf::Gperf``
#     The gperf executable.
#
# and the following public function:
#
#   ecm_gperf_generate(<GperfInput> <OutputFile> <OutputVariable>
#                      [GENERATION_FLAGS <flags>])
#
# Run ``gperf`` on ``<GperfInput>`` to generate ``<OutputFile>``, adding it to
# the ``<OutputVariable>`` variable which contains the source for the target
# where ``<OutputFile>`` is going to be built.  The optional
# ``GENERATION_FLAGS`` argument is needed to pass extra parameters to
# ``gperf`` (note you cannot override that way the output file).
#
# A simple invocation would be:
#
# .. code-block:: cmake
#
#   ecm_gperf_generate(simple.gperf ${CMAKE_CURRENT_BINARY_DIR}/simple.h MySources)
#
# Since 5.35.0.

#=============================================================================
# Copyright 2016-2017 Pino Toscano <pino@kde.org>
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

ecm_find_package_version_check(Gperf)

# Find gperf
find_program(Gperf_EXECUTABLE NAMES gperf)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gperf
    FOUND_VAR
        Gperf_FOUND
    REQUIRED_VARS
        Gperf_EXECUTABLE
)

mark_as_advanced(Gperf_EXECUTABLE)

if (Gperf_FOUND)
    if (NOT TARGET GPerf::Gperf)
        add_executable(GPerf::Gperf IMPORTED)
        set_target_properties(GPerf::Gperf PROPERTIES
            IMPORTED_LOCATION "${Gperf_EXECUTABLE}"
        )
    endif()
endif()

include(FeatureSummary)
set_package_properties(Gperf PROPERTIES
    URL "https://www.gnu.org/software/gperf/"
    DESCRIPTION "Perfect hash function generator"
)


include(CMakeParseArguments)

function(ecm_gperf_generate input_file output_file out_var)
    # Parse arguments
    set(oneValueArgs GENERATION_FLAGS)
    cmake_parse_arguments(ARGS "" "${oneValueArgs}" "" ${ARGN})

    if(ARGS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to ecm_gperf_generate(): \"${ARGS_UNPARSED_ARGUMENTS}\"")
    endif()

    get_filename_component(_infile ${input_file} ABSOLUTE)
    set(_extraopts "${ARGS_GENERATION_FLAGS}")
    separate_arguments(_extraopts)
    add_custom_command(OUTPUT ${output_file}
        COMMAND ${Gperf_EXECUTABLE} ${_extraopts} --output-file=${output_file} ${_infile}
        DEPENDS ${_infile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
    )
    set_property(SOURCE ${output_file} PROPERTY SKIP_AUTOMOC ON)

    list(APPEND ${out_var} "${output_file}")
    set(${out_var} ${${out_var}} PARENT_SCOPE)
endfunction()
