include("${CMAKE_CURRENT_LIST_DIR}/projects.cmake")

function(make_static_imported_library proj libfile_name)
  add_library(SuperBuild::${proj} STATIC IMPORTED GLOBAL)
  
  # set INTERFACE_INCLUDE_DIRECTORIES instead of INCLUDE_DIRECTORIES on the library
  # see: https://stackoverflow.com/questions/25907478/possible-to-add-an-imported-library-to-target-link-libraries-that-takes-care-of
  # explicitly set imported locations per config that may be generated for the library
  set_target_properties(SuperBuild::${proj} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/${proj}/lib/${libfile_name}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/${proj}/lib/${libfile_name}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/${proj}/lib/${libfile_name}"
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${PROJECT_BUILD_DIR}/x64/\$<CONFIG>/${proj}/include/>"
  )
endfunction()

# create library targets for each added third party project

if(abseil-cpp IN_LIST projects)
  make_static_imported_library(abseil-cpp ${CMAKE_STATIC_LIBRARY_PREFIX}abseil-cpp${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

if(protobuf IN_LIST projects)
  make_static_imported_library(protobuf ${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX})
  add_dependencies(SuperBuild::protobuf SuperBuild::abseil-cpp)
endif()
