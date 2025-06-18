include("${CMAKE_CURRENT_LIST_DIR}/projects.cmake")
set(CMAKE_PREFIX_PATH "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}")

if (UNIX)
  find_package(PkgConfig REQUIRED)
endif()

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

function(make_static_imported_library libfile_name target_name debug_string)
  add_library(SuperBuild::${target_name} STATIC IMPORTED GLOBAL)
  set_target_properties(SuperBuild::${target_name} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/lib/${libfile_name}${debug_string}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/lib/${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/lib/${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
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
  set(sfml_dependency_debug_string "d")
  set(sfml_debug_string "-d")
  target_link_libraries(SuperBuild::SFML INTERFACE
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-graphics-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-window-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-audio-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-network-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
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
  add_library(SFML INTERFACE) # on linux, there is no libsfml-main.a file
  add_library(SuperBuild::SFML ALIAS SFML)
  set_target_properties(SFML PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/include/"
    INTERFACE_COMPILE_DEFINITIONS SFML_STATIC
  )
  # to statically link SFML, you need to manually link all dependencies, which are included library files in SFML
  set(sfml_dependency_debug_string "d")
  set(sfml_debug_string "-d")
  pkg_check_modules(X11 REQUIRED x11)
  pkg_check_modules(XRANDR REQUIRED xrandr)
  pkg_check_modules(XCURSOR REQUIRED xcursor)
  pkg_check_modules(XI REQUIRED xi)
  pkg_check_modules(FREETYPE REQUIRED freetype2)
  pkg_check_modules(UDEV REQUIRED libudev)
  target_link_libraries(SFML INTERFACE
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-graphics-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-window-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-audio-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-network-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-system-s$<$<CONFIG:Debug>:${sfml_debug_string}>${CMAKE_STATIC_LIBRARY_SUFFIX}"
    ${X11_STATIC_LIBRARIES}
    ${XRANDR_STATIC_LIBRARIES}
    ${XCURSOR_STATIC_LIBRARIES}
    ${XI_STATIC_LIBRARIES}
    ${FREETYPE_STATIC_LIBRARIES}
    ${UDEV_STATIC_LIBRARIES}
  )
endif()

if(zlib IN_LIST projects)
  if(WIN32)
    top_level_static_imported_library(zlib zlibstatic "d")
  elseif(UNIX)
    top_level_static_imported_library(zlib z "")
  endif()
endif()

if(protobuf IN_LIST projects)
  set(protobuf_debug_string "d")
  find_package(absl)
  # all protobuf lib files have prefix "lib"
  make_static_imported_library(libprotobuf libprotobuf ${protobuf_debug_string})
  make_static_imported_library(libprotobuf-lite libprotobuf-lite ${protobuf_debug_string})
  make_static_imported_library(libprotoc libprotoc ${protobuf_debug_string})
  make_static_imported_library(libutf8_range libutf8_range "")
  make_static_imported_library(libutf8_validity libutf8_validity "")

  add_dependencies(SuperBuild::libprotobuf SuperBuild::zlib)

  set_target_properties(SuperBuild::libprotobuf-lite PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/" INTERFACE_LINK_LIBRARIES "absl::absl_check;absl::absl_log;absl::algorithm;absl::base;absl::bind_front;absl::bits;absl::btree;absl::cleanup;absl::cord;absl::core_headers;absl::debugging;absl::die_if_null;absl::dynamic_annotations;absl::flags;absl::flat_hash_map;absl::flat_hash_set;absl::function_ref;absl::hash;absl::layout;absl::log_initialize;absl::log_globals;absl::log_severity;absl::memory;absl::node_hash_map;absl::node_hash_set;absl::optional;absl::random_distributions;absl::random_random;absl::span;absl::status;absl::statusor;absl::strings;absl::synchronization;absl::time;absl::type_traits;absl::utility;absl::variant;\$<LINK_ONLY:utf8_range::utf8_validity>")
  
  set_target_properties(SuperBuild::libprotoc PROPERTIES INTERFACE_LINK_LIBRARIES "\$<LINK_ONLY:protobuf::libprotobuf>;absl::absl_check;absl::absl_log;absl::algorithm;absl::base;absl::bind_front;absl::bits;absl::btree;absl::cleanup;absl::cord;absl::core_headers;absl::debugging;absl::die_if_null;absl::dynamic_annotations;absl::flags;absl::flat_hash_map;absl::flat_hash_set;absl::function_ref;absl::hash;absl::layout;absl::log_initialize;absl::log_globals;absl::log_severity;absl::memory;absl::node_hash_map;absl::node_hash_set;absl::optional;absl::random_distributions;absl::random_random;absl::span;absl::status;absl::statusor;absl::strings;absl::synchronization;absl::time;absl::type_traits;absl::utility;absl::variant")

  set_target_properties(SuperBuild::libprotobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/" INTERFACE_LINK_LIBRARIES "absl::absl_check;absl::absl_log;absl::algorithm;absl::base;absl::bind_front;absl::bits;absl::btree;absl::cleanup;absl::cord;absl::core_headers;absl::debugging;absl::die_if_null;absl::dynamic_annotations;absl::flags;absl::flat_hash_map;absl::flat_hash_set;absl::function_ref;absl::hash;absl::layout;absl::log_initialize;absl::log_globals;absl::log_severity;absl::memory;absl::node_hash_map;absl::node_hash_set;absl::optional;absl::random_distributions;absl::random_random;absl::span;absl::status;absl::statusor;absl::strings;absl::synchronization;absl::time;absl::type_traits;absl::utility;absl::variant"
  )
  target_link_libraries(SuperBuild::libprotobuf INTERFACE SuperBuild::zlib SuperBuild::libutf8_range SuperBuild::libutf8_validity)
endif()

if(boringssl IN_LIST projects)
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}crypto crypto "")
  add_library(SuperBuild::openssl STATIC IMPORTED GLOBAL)
  set_target_properties(SuperBuild::openssl PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
  )
  set_target_properties(SuperBuild::openssl PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
  add_dependencies(SuperBuild::openssl SuperBuild::crypto)
  target_link_libraries(SuperBuild::openssl INTERFACE SuperBuild::crypto)
endif()

if(cares IN_LIST projects)
  top_level_static_imported_library(cares cares "")
endif()

if(re2 IN_LIST projects)
  top_level_static_imported_library(re2 re2 "")
endif()

if(grpc IN_LIST projects)
  find_package(absl)
  # establishing upb dependency libraries
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_base_lib upb_base_lib "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_json_lib upb_json_lib "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_mem_lib upb_mem_lib "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_message_lib upb_message_lib "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_mini_descriptor_lib upb_mini_descriptor_lib "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_textformat_lib upb_textformat_lib "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}upb_wire_lib upb_wire_lib "")
  add_library(upb INTERFACE)
  add_library(SuperBuild::upb ALIAS upb)
  target_link_libraries(upb INTERFACE SuperBuild::upb_textformat_lib SuperBuild::upb_wire_lib SuperBuild::upb_message_lib 
    SuperBuild::upb_mini_descriptor_lib SuperBuild::upb_json_lib SuperBuild::upb_mem_lib SuperBuild::upb_base_lib
  )

  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}gpr gpr "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}grpc grpc "")
  top_level_static_imported_library(grpc++ grpc++ "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}address_sorting address_sorting "")
  
  add_dependencies(SuperBuild::grpc++ SuperBuild::grpc SuperBuild::gpr 
    SuperBuild::upb SuperBuild::address_sorting SuperBuild::zlib
    SuperBuild::libprotobuf SuperBuild::openssl SuperBuild::cares SuperBuild::re2
  )

  set_target_properties(SuperBuild::gpr PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/" INTERFACE_LINK_LIBRARIES "absl::base;absl::core_headers;absl::log_severity;absl::cleanup;absl::flags;absl::flags_marshalling;absl::any_invocable;absl::check;absl::log_globals;absl::log;absl::memory;absl::bits;absl::random_random;absl::status;absl::cord;absl::str_format;absl::strings;absl::synchronization;absl::time;absl::optional")
  
  set_target_properties(SuperBuild::grpc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/" INTERFACE_LINK_LIBRARIES "SuperBuild::upb;SuperBuild::re2;SuperBuild::zlib;absl::algorithm_container;absl::config;absl::no_destructor;absl::flat_hash_map;absl::flat_hash_set;absl::inlined_vector;absl::bind_front;absl::function_ref;absl::hash;absl::type_traits;absl::random_bit_gen_ref;absl::random_distributions;absl::statusor;absl::span;absl::utility;SuperBuild::cares;SuperBuild::gpr;SuperBuild::openssl;SuperBuild::address_sorting")

  set_target_properties(SuperBuild::grpc++ PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/" INTERFACE_LINK_LIBRARIES "absl::absl_check;absl::absl_log")
  target_link_libraries(SuperBuild::grpc++ INTERFACE SuperBuild::grpc SuperBuild::libprotobuf)
endif()
