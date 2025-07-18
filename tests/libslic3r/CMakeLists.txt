get_filename_component(_TEST_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

add_executable(${_TEST_NAME}_tests 
	${_TEST_NAME}_tests.cpp
	test_3mf.cpp
	test_aabbindirect.cpp
	test_clipper_offset.cpp
	test_clipper_utils.cpp
	test_config.cpp
	test_elephant_foot_compensation.cpp
	test_geometry.cpp
	test_placeholder_parser.cpp
	test_polygon.cpp
	test_mutable_polygon.cpp
	test_mutable_priority_queue.cpp
	test_stl.cpp
	test_meshboolean.cpp
	test_marchingsquares.cpp
	test_timeutils.cpp
	test_voronoi.cpp
    test_optimizers.cpp
    test_png_io.cpp
    test_timeutils.cpp
        test_indexed_triangle_set.cpp
        test_buildplate_manager.cpp
        ../libnest2d/printer_parts.cpp
        )

if (TARGET OpenVDB::openvdb)
    target_sources(${_TEST_NAME}_tests PRIVATE test_hollowing.cpp)
endif()
    
target_link_libraries(${_TEST_NAME}_tests test_common libslic3r)
set_property(TARGET ${_TEST_NAME}_tests PROPERTY FOLDER "tests")

if (WIN32)
    bambuslicer_copy_dlls(${_TEST_NAME}_tests)
endif()

# catch_discover_tests(${_TEST_NAME}_tests TEST_PREFIX "${_TEST_NAME}: ")
add_test(${_TEST_NAME}_tests ${_TEST_NAME}_tests ${CATCH_EXTRA_ARGS})
