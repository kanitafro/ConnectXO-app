cmake_minimum_required(VERSION 3.17)

set(SOLUTION_NAME ConnectXO)

project(${SOLUTION_NAME})

# ==============================================================================
# CONTINUATION OF SOLUTIONS & ENVIRONMENT SETUP
# ==============================================================================
set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})
if (WIN32)
	set(HOME_ROOT $ENV{USERPROFILE})
	string(REPLACE "\\" "/" HOME_ROOT "${HOME_ROOT}")
else()
	set(HOME_ROOT $ENV{HOME})
endif()
set(NATID_SDK_ROOT ${HOME_ROOT}/natID.SDK)

include(${NATID_SDK_ROOT}/DevEnv/Common.cmake) 
include(${NATID_SDK_ROOT}/DevEnv/MatrixLib.cmake)
include(${NATID_SDK_ROOT}/DevEnv/natGUI.cmake)

# ==============================================================================
# PACKAGING LOGIC (Placed safely before the local project builds)
# ==============================================================================
set(CPACK_PACKAGE_NAME "ConnectXO")
set(CPACK_PACKAGE_VENDOR "Kanitafro")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ConnectXO Desktop Application")

if(WIN32)
    set(CPACK_GENERATOR "INNOSETUP")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "ConnectXO")
    set(CPACK_INNOSETUP_CREATE_DESKTOP_ICON ON)

    # Force BOTH variable hooks so CPack cannot fail its initialization phase
    set(CPACK_INNOSETUP_COMPILER "C:/Users/Korisnik/AppData/Local/Programs/Inno Setup 6/ISCC.exe" CACHE FILEPATH "Path to Inno Setup compiler" FORCE)
    set(CPACK_INNOSETUP_EXECUTABLE "C:/Users/Korisnik/AppData/Local/Programs/Inno Setup 6/ISCC.exe" CACHE FILEPATH "Path to Inno Setup executable" FORCE)
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "ConnectXO Installer")
endif()

include(CPack)

# ==============================================================================
# TARGET BUILD DEFINITIONS
# ==============================================================================
include(ConnectXO.cmake)