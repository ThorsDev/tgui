add_subdirectory(tgui)
add_subdirectory(platform)
add_subdirectory(renderer)

if(WIN32)
	set_target_properties(tgui_renderer PROPERTIES FOLDER tgui/lib)
	set_target_properties(tgui_platform PROPERTIES FOLDER tgui/lib)
	
	set_target_properties(tgui PROPERTIES FOLDER tgui/lib)
endif()