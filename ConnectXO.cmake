set(CONNECTXO_NAME ConnectXO)				#Naziv prvog projekta u solution-u

file(GLOB CONNECTXO_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB CONNECTXO_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB CONNECTXO_INC_TD  ${MY_INC}/td/*.h)
file(GLOB CONNECTXO_INC_GUI ${MY_INC}/gui/*.h)

file(GLOB CONNECTXO_INC_THREAD  ${MY_INC}/thread/*.h)
file(GLOB CONNECTXO_INC_CNT  ${MY_INC}/cnt/*.h)
file(GLOB CONNECTXO_INC_FO  ${MY_INC}/fo/*.h)
file(GLOB CONNECTXO_INC_XML  ${MY_INC}/xml/*.h)

#Application icon
set(CONNECTXO_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(CONNECTXO_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(CONNECTXO_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${CONNECTXO_NAME} ${CONNECTXO_INCS} ${CONNECTXO_SOURCES} ${CONNECTXO_INC_TD} ${CONNECTXO_INC_THREAD} 
				${CONNECTXO_INC_CNT} ${CONNECTXO_INC_FO} ${CONNECTXO_INC_GUI} ${CONNECTXO_INC_XML} ${CONNECTXO_WINAPP_ICON})

source_group("inc"            FILES ${CONNECTXO_INCS})
source_group("inc\\td"        FILES ${CONNECTXO_INC_TD})
source_group("inc\\cnt"        FILES ${CONNECTXO_INC_CNT})
source_group("inc\\fo"        FILES ${CONNECTXO_INC_FO})
source_group("inc\\gui"        FILES ${CONNECTXO_INC_GUI})
source_group("inc\\thread"        FILES ${CONNECTXO_INC_THREAD})
source_group("inc\\xml"        FILES ${CONNECTXO_INC_XML})
source_group("src"            FILES ${CONNECTXO_SOURCES})

target_link_libraries(${CONNECTXO_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${CONNECTXO_NAME} ${CONNECTXO_PLIST})

setAppIcon(${CONNECTXO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${CONNECTXO_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${CONNECTXO_NAME})
