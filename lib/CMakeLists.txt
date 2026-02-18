add_subdirectory(renderer)
add_subdirectory(tgui)

if(WIN32)
	set_target_properties(tgui_renderer PROPERTIES FOLDER tgui/lib)
	set_target_properties(tgui PROPERTIES FOLDER tgui/lib)
endif()