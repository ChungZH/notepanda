#.rst:
# FindPhoneNumber
# ---------------
#
# Try to find PhoneNumber.
#
# This is a component-based find module, which makes use of the COMPONENTS and
# OPTIONAL_COMPONENTS arguments to find_module.  The following components are
# available::
#
#   PhoneNumber  GeoCoding
#
# If no components are specified, this module will act as though all components
# were passed to OPTIONAL_COMPONENTS.
#
# This module will define the following variables, independently of the
# components searched for or found:
#
# ``PhoneNumber_FOUND``
#     True if (the requestion version of) PhoneNumber is available
#
# For each searched-for components, ``PhoneNumber_<component>_FOUND`` will be set to
# TRUE if the corresponding library was found, and FALSE otherwise.  If
# ``PhoneNumber_<component>_FOUND`` is TRUE, the imported target ``PhoneNumber::<component>``
# will be defined.
#
# Since 5.54.0.

#=============================================================================
# Copyright (c) 2017 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
# Copyright (c) 2018 Volker Krause <vkrause@kde.org>
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

include(ECMFindModuleHelpersStub)

ecm_find_package_version_check(PhoneNumber)

set(PhoneNumber_known_components
    PhoneNumber
    GeoCoding
)
set(PhoneNumber_default_components ${PhoneNumber_known_components})

set(PhoneNumber_PhoneNumber_lib phonenumber)
set(PhoneNumber_PhoneNumber_header phonenumbers/phonenumberutil.h)
set(PhoneNumber_GeoCoding_lib geocoding)
set(PhoneNumber_GeoCoding_header phonenumbers/geocoding/phonenumber_offline_geocoder.h)

ecm_find_package_parse_components(PhoneNumber
    RESULT_VAR PhoneNumber_components
    KNOWN_COMPONENTS ${PhoneNumber_known_components}
    DEFAULT_COMPONENTS ${PhoneNumber_default_components}
)
ecm_find_package_handle_library_components(PhoneNumber
    COMPONENTS ${PhoneNumber_components}
)
find_package_handle_standard_args(PhoneNumber
    FOUND_VAR
        PhoneNumber_FOUND
    REQUIRED_VARS
        PhoneNumber_LIBRARIES
    VERSION_VAR
        PhoneNumber_VERSION
    HANDLE_COMPONENTS
)
set(PhoneNumber_VERSION 0) # there is no observable version number in the installed files

include(FeatureSummary)
set_package_properties(PhoneNumber PROPERTIES
  URL "https://github.com/googlei18n/libphonenumber"
  DESCRIPTION "Library for parsing, formatting, and validating international phone numbers")
