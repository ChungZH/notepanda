#.rst:
# ECMGenerateQmlTypes
# -------------------
#
# Generates plugins.qmltypes files for QML plugins.
#
# ::
#
#   ecm_generate_qmltypes(<org.kde.pluginname> 1.3
#                         DESTINATION <${KDE_INSTALL_QMLDIR}/org/kde/pluginname>)
#
# Makes it possible to generate plugins.qmltypes files for the QML plugins that
# our project offers. These files offer introspection upon our plugin and are
# useful for integrating with IDE language support of our plugin. It offers
# information about the objects its methods and their argument types.
#
# The developer will be in charge of making sure that these files are up to date.
# The plugin.qmltypes file will sit in the source directory. This function will
# include the code that installs the file in the right place and a small unit
# test named qmltypes-pluginname-version that makes sure that it doesn't need updating.
#
#
# Since 5.33.0

#=============================================================================
# Copyright 2017 Aleix Pol Gonzalez <aleixpol@kde.org>
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

include(CMakeParseArguments)
function(ecm_generate_qmltypes)
    if (NOT TARGET qmltypes)
        add_custom_target(qmltypes)
    endif ()

    set(options)
    set(oneValueArgs DESTINATION TEST_ENABLED)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

    set(targetname "qmltypes-${ARG_UNPARSED_ARGUMENTS}")
    string(REPLACE ";" - targetname "${targetname}")

    set(generatedFile ${CMAKE_CURRENT_SOURCE_DIR}/plugins.qmltypes)
    add_custom_target(${targetname}
        BYPRODUCTS ${generatedFile}
        COMMAND qmlplugindump -nonrelocatable ${ARG_UNPARSED_ARGUMENTS} ${KDE_INSTALL_QMLDIR} > ${generatedFile}
    )
    add_dependencies(qmltypes ${targetname})
    if (EXISTS ${generatedFile})
        install(FILES ${generatedFile} DESTINATION "${ARG_DESTINATION}")
    endif()

    string(REPLACE ";" / processedArgs "${ARG_UNPARSED_ARGUMENTS}")

    # sometimes qmlplugindump output isn't reproducible, we better have it opt in for now
    if(ARG_TEST_ENABLED)
        add_test(NAME ${targetname} COMMAND
            cmake -DARG_UNPARSED_ARGUMENTS=${processedArgs} -DKDE_INSTALL_QMLDIR=${KDE_INSTALL_QMLDIR} -DINPUT=${generatedFile} -P ${ECM_MODULE_DIR}/../test-modules/test_execute_and_compare.cmake
        )
    endif()
endfunction()
