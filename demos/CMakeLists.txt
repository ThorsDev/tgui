add_subdirectory(demo-test)

if (WIN32)
	set_target_properties(tgui_demo_test PROPERTIES FOLDER tgui/demos)
endif()