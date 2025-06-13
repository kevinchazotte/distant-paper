include("${CMAKE_CURRENT_LIST_DIR}/projects.cmake")

function(top_level_static_imported_library proj file_name debug_string)
  add_library(SuperBuild::${proj} STATIC IMPORTED GLOBAL)
  
  # set INTERFACE_INCLUDE_DIRECTORIES instead of INCLUDE_DIRECTORIES on the library
  # see: https://stackoverflow.com/questions/25907478/possible-to-add-an-imported-library-to-target-link-libraries-that-takes-care-of
  # explicitly set imported locations per config that may be generated for the library
  set_target_properties(SuperBuild::${proj} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${file_name}${debug_string}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${file_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${file_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )
  set_target_properties(SuperBuild::${proj} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
endfunction()

function(make_static_imported_library libfile_name debug_string)
  add_library(SuperBuild::${libfile_name} STATIC IMPORTED GLOBAL)
  set_target_properties(SuperBuild::${libfile_name} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${libfile_name}${debug_string}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )
endfunction()

# create library targets for each added third party project

# SFML has different Mac/UNIX behavior AND different lib files for static linking and debug configs
# see https://www.sfml-dev.org/tutorials/2.5/start-vc.php
if(WIN32 AND "SFML" IN_LIST projects)
  add_library(SuperBuild::SFML STATIC IMPORTED GLOBAL)
  set_target_properties(SuperBuild::SFML PROPERTIES
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-main${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-main${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-main-d${CMAKE_STATIC_LIBRARY_SUFFIX}"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/include/"
    INTERFACE_COMPILE_DEFINITIONS SFML_STATIC
  )
  # to statically link SFML, you need to manually link all dependencies, which are included library files in SFML
  # TODO: use SFML_USE_SYSTEM_DEPS to remove these dependencies and add them to the SuperBuild independently of SFML
  # TODO: add UNIX and MAC support for the library, which should be (largely) consistent with the above 
  set(sfml_dependency_debug_string "d")
  set(sfml_debug_string "-d")
  target_link_libraries(SuperBuild::SFML INTERFACE
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-audio-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-network-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-graphics-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-window-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-system-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}flac$<$<CONFIG:Debug>:${sfml_dependency_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freetype$<$<CONFIG:Debug>:${sfml_dependency_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ogg$<$<CONFIG:Debug>:${sfml_dependency_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbis$<$<CONFIG:Debug>:${sfml_dependency_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisenc$<$<CONFIG:Debug>:${sfml_dependency_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisfile$<$<CONFIG:Debug>:${sfml_dependency_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    ws2_32.lib
    winmm.lib
    opengl32.lib
  )
elseif(UNIX AND "SFML" IN_LIST projects)
  make_static_imported_library(SFML "-d") # temporary library scaffolding while UNIX kinks are worked out
endif()

if(abseil-cpp IN_LIST projects)
  add_library(abseil-cpp INTERFACE)
  add_library(SuperBuild::abseil-cpp ALIAS abseil-cpp)
  file(GLOB ABSEIL_LIBS
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}absl_*${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )
  foreach(absl_lib ${ABSEIL_LIBS})
    get_filename_component(libname ${absl_lib} NAME_WE)
    make_static_imported_library(${libname} "")
    target_link_libraries(abseil-cpp INTERFACE SuperBuild::${libname})
  endforeach()
  set_target_properties(abseil-cpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
endif()

if(zlib IN_LIST projects)
  top_level_static_imported_library(zlib zlibstatic "d")
endif()

if(boringssl IN_LIST projects)
  make_static_imported_library(crypto "")
  top_level_static_imported_library(openssl ssl "")
  add_dependencies(SuperBuild::openssl SuperBuild::crypto)
  target_link_libraries(SuperBuild::openssl INTERFACE SuperBuild::crypto)
endif()

if(cares IN_LIST projects)
  top_level_static_imported_library(cares cares "")
endif()

if(re2 IN_LIST projects)
  top_level_static_imported_library(re2 re2 "")
  add_dependencies(SuperBuild::re2 SuperBuild::abseil-cpp)
  target_link_libraries(SuperBuild::re2 INTERFACE SuperBuild::abseil-cpp)
endif()

if(protobuf IN_LIST projects)
  set(protobuf_debug_string "d")
  make_static_imported_library(libprotobuf ${protobuf_debug_string})
  make_static_imported_library(libprotobuf-lite ${protobuf_debug_string})
  make_static_imported_library(libprotoc ${protobuf_debug_string})
  make_static_imported_library(libutf8_range "")
  make_static_imported_library(libutf8_validity "")
  add_library(protobuf INTERFACE)
  add_library(SuperBuild::protobuf ALIAS protobuf)
  add_dependencies(protobuf SuperBuild::abseil-cpp SuperBuild::zlib)
  set_target_properties(protobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
  target_link_libraries(protobuf INTERFACE SuperBuild::abseil-cpp SuperBuild::zlib SuperBuild::libprotobuf
    SuperBuild::libprotobuf-lite SuperBuild::libprotoc SuperBuild::libutf8_range SuperBuild::libutf8_validity
  )
endif()

if(grpc IN_LIST projects)
  # establishing upb dependency libraries
  make_static_imported_library(upb_base_lib "")
  make_static_imported_library(upb_json_lib "")
  make_static_imported_library(upb_mem_lib "")
  make_static_imported_library(upb_message_lib "")
  make_static_imported_library(upb_mini_descriptor_lib "")
  make_static_imported_library(upb_textformat_lib "")
  make_static_imported_library(upb_wire_lib "")
  add_library(upb INTERFACE)
  add_library(SuperBuild::upb ALIAS upb)
  target_link_libraries(upb INTERFACE SuperBuild::upb_base_lib SuperBuild::upb_json_lib SuperBuild::upb_mem_lib
    SuperBuild::upb_message_lib SuperBuild::upb_mini_descriptor_lib SuperBuild::upb_textformat_lib SuperBuild::upb_wire_lib
  )
  make_static_imported_library(address_sorting "")
  make_static_imported_library(grpc++ "")
  make_static_imported_library(gpr "")
  top_level_static_imported_library(grpc grpc "")
  add_dependencies(SuperBuild::grpc SuperBuild::grpc++ SuperBuild::gpr 
	SuperBuild::upb SuperBuild::address_sorting SuperBuild::zlib
    SuperBuild::protobuf SuperBuild::openssl SuperBuild::cares SuperBuild::re2
  )
  target_link_libraries(SuperBuild::grpc INTERFACE SuperBuild::grpc++ SuperBuild::gpr
    SuperBuild::upb SuperBuild::address_sorting SuperBuild::zlib
    SuperBuild::protobuf SuperBuild::openssl SuperBuild::cares SuperBuild::re2
  )
endif()
