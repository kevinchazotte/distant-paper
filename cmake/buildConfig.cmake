include("${CMAKE_CURRENT_LIST_DIR}/projects.cmake")

function(make_static_imported_library proj libfile_name debug_string)
  add_library(SuperBuild::${libfile_name} STATIC IMPORTED GLOBAL)
  
  # set INTERFACE_INCLUDE_DIRECTORIES instead of INCLUDE_DIRECTORIES on the library
  # see: https://stackoverflow.com/questions/25907478/possible-to-add-an-imported-library-to-target-link-libraries-that-takes-care-of
  # explicitly set imported locations per config that may be generated for the library
  set_target_properties(SuperBuild::${libfile_name} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/${proj}/lib/${libfile_name}${debug_string}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/${proj}/lib/${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/${proj}/lib/${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )
endfunction()

# create library targets for each added third party project

if(abseil-cpp IN_LIST projects)
  add_library(abseil-cpp INTERFACE)
  add_library(SuperBuild::abseil-cpp ALIAS abseil-cpp)
  file(GLOB ABSEIL_LIBS
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/abseil-cpp/lib/${CMAKE_STATIC_LIBRARY_PREFIX}absl_*.lib"
  )
  foreach(absl_lib ${ABSEIL_LIBS})
    get_filename_component(libname ${absl_lib} NAME_WE)
    make_static_imported_library(abseil-cpp ${libname} "")
    target_link_libraries(abseil-cpp INTERFACE SuperBuild::${libname})
  endforeach()
  set_target_properties(abseil-cpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/abseil-cpp/include/"
  )
endif()

if(protobuf IN_LIST projects)
  set(protobuf_debug_string "d")
  make_static_imported_library(protobuf libprotobuf ${protobuf_debug_string})
  make_static_imported_library(protobuf libprotobuf-lite ${protobuf_debug_string})
  make_static_imported_library(protobuf libprotoc ${protobuf_debug_string})
  make_static_imported_library(protobuf libupb ${protobuf_debug_string})
  make_static_imported_library(protobuf libutf8_range "")
  make_static_imported_library(protobuf libutf8_validity "")
  add_library(protobuf INTERFACE)
  add_library(SuperBuild::protobuf ALIAS protobuf)
  add_dependencies(protobuf SuperBuild::abseil-cpp)
  set_target_properties(protobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/protobuf/include/"
  )
  target_link_libraries(protobuf INTERFACE SuperBuild::abseil-cpp
    SuperBuild::libprotobuf SuperBuild::libprotobuf-lite SuperBuild::libprotoc SuperBuild::libupb SuperBuild::libutf8_range SuperBuild::libutf8_validity
  )
endif()
