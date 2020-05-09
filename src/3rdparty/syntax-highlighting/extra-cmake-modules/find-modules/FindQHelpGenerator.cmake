# WARNING: FOR ECM-INTERNAL USE ONLY, DO NOT USE IN OWN PROJECTS
# THIS FILE MIGHT DISAPPEAR IN FUTURE VERSIONS OF ECM.

# Finds the Qt5 QHelpGenerator
#
#  QHelpGenerator_FOUND     - True if QHelpGenerator found.
#  QHelpGenerator_EXECUTABLE - Path to executable

#=============================================================================
# Copyright 2016 Friedrich W. H. Kossebau <kossebau@kde.org>
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

find_package(Qt5Help QUIET)
if (TARGET Qt5::qhelpgenerator)
    get_target_property(QHelpGenerator_EXECUTABLE Qt5::qhelpgenerator LOCATION)
else()
    # assume same folder as qmake executable
    if (TARGET Qt5::qmake)
        get_target_property(_qmake_EXECUTABLE Qt5::qmake LOCATION)
        get_filename_component(_path ${_qmake_EXECUTABLE} DIRECTORY)
    else()
        set(_path)
    endif()
    find_program(QHelpGenerator_EXECUTABLE
        NAMES
            qhelpgenerator-qt5
            qhelpgenerator
        PATHS
            ${_path}
        NO_DEFAULT_PATH
    )
endif()
mark_as_advanced(QHelpGenerator_EXECUTABLE)

if(QHelpGenerator_EXECUTABLE)
    set(QHelpGenerator_FOUND TRUE)
else()
    set(QHelpGenerator_FOUND FALSE)
endif()

if(QHelpGenerator_FOUND)
  if(NOT QHelpGenerator_FIND_QUIETLY )
    message( STATUS "Found QHelpGenerator executable: ${QHelpGenerator_EXECUTABLE}")
  endif()
else()
  if(QHelpGenerator_FIND_REQUIRED)
    message( FATAL_ERROR "Could not find QHelpGenerator executable" )
  else()
    message( STATUS "Optional QHelpGenerator executable was not found" )
  endif()
endif()

