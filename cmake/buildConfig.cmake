include("${CMAKE_CURRENT_LIST_DIR}/projects.cmake")

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

if(abseil-cpp IN_LIST projects)
  add_library(abseil-cpp INTERFACE)
  add_library(SuperBuild::abseil-cpp ALIAS abseil-cpp)
  set(ABSL_LIBS absl_bad_any_cast_impl absl_bad_optional_access absl_bad_variant_access absl_base absl_city
    absl_civil_time absl_cord absl_cord_internal absl_cordz_functions absl_cordz_handle absl_cordz_info
    absl_cordz_sample_token absl_crc32c absl_crc_cord_state absl_crc_cpu_detect absl_crc_internal
    absl_debugging_internal absl_decode_rust_punycode absl_demangle_internal absl_demangle_rust absl_die_if_null
    absl_examine_stack absl_exponential_biased absl_failure_signal_handler absl_flags_commandlineflag
    absl_flags_commandlineflag_internal absl_flags_config absl_flags_internal absl_flags_marshalling absl_flags_parse
    absl_flags_private_handle_accessor absl_flags_program_name absl_flags_reflection absl_flags_usage
    absl_flags_usage_internal absl_graphcycles_internal absl_hash absl_hashtablez_sampler absl_int128
    absl_kernel_timeout_internal absl_leak_check absl_log_entry absl_log_flags absl_log_globals absl_log_initialize
    absl_log_internal_check_op absl_log_internal_conditions absl_log_internal_fnmatch absl_log_internal_format
    absl_log_internal_globals absl_log_internal_log_sink_set absl_log_internal_message absl_log_internal_nullguard
    absl_log_internal_proto absl_log_internal_structured_proto absl_log_severity absl_log_sink absl_low_level_hash
    absl_malloc_internal absl_periodic_sampler absl_poison absl_random_distributions
    absl_random_internal_distribution_test_util absl_random_internal_platform absl_random_internal_pool_urbg
    absl_random_internal_randen absl_random_internal_randen_hwaes absl_random_internal_randen_hwaes_impl
    absl_random_internal_randen_slow absl_random_internal_seed_material absl_random_seed_gen_exception
    absl_random_seed_sequences absl_raw_hash_set absl_raw_logging_internal absl_scoped_set_env absl_spinlock_wait
    absl_stacktrace absl_status absl_statusor absl_str_format_internal absl_strerror absl_string_view absl_strings
    absl_strings_internal absl_symbolize absl_synchronization absl_throw_delegate absl_time absl_time_zone
    absl_tracing_internal absl_utf8_for_code_point absl_vlog_config_internal
  )
  set(ABSL_LIB_PATHS)
  foreach(lib_name IN LISTS ABSL_LIBS)
    list(APPEND ABSL_LIB_PATHS "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${lib_name}${CMAKE_STATIC_LIBRARY_SUFFIX}")
  endforeach()
  # abseil-cpp build generates dozens of lib files, instead of linking manually with correct dependency order:
  # use linker --start-group and --end-group commands to allow linker to resolve circular dependencies among absl libs
  target_link_libraries(abseil-cpp INTERFACE
    -Wl,--start-group
    ${ABSL_LIB_PATHS}
    -Wl,--end-group
  )
  find_package(Threads)
  if(${Threads_FOUND})
    target_link_libraries(abseil-cpp INTERFACE Threads::Threads)
  endif()
  set_target_properties(abseil-cpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
endif()

if(zlib IN_LIST projects)
  if(WIN32)
    top_level_static_imported_library(zlib zlibstatic "d")
  elseif(UNIX)
    top_level_static_imported_library(zlib z "")
  endif()
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
  add_dependencies(SuperBuild::re2 SuperBuild::abseil-cpp)
  target_link_libraries(SuperBuild::re2 INTERFACE SuperBuild::abseil-cpp)
endif()

if(protobuf IN_LIST projects)
  set(protobuf_debug_string "d")
  # all protobuf lib files have prefix "lib"
  if (WIN32) # on Windows, manually set libfile names to include "lib"
    make_static_imported_library(libprotobuf libprotobuf ${protobuf_debug_string})
    make_static_imported_library(libprotobuf-lite libprotobuf-lite ${protobuf_debug_string})
    make_static_imported_library(libprotoc libprotoc ${protobuf_debug_string})
    make_static_imported_library(libutf8_range libutf8_range "")
    make_static_imported_library(libutf8_validity libutf8_validity "")
  elseif(UNIX) # on Unix, "lib" is the default CMAKE_STATIC_LIBRARY_PREFIX
    make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}protobuf libprotobuf ${protobuf_debug_string})
    make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}protobuf-lite libprotobuf-lite ${protobuf_debug_string})
    make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}protoc libprotoc ${protobuf_debug_string})
    make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}utf8_range libutf8_range "")
    make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}utf8_validity libutf8_validity "")
  endif()
  add_library(protobuf INTERFACE)
  add_library(SuperBuild::protobuf ALIAS protobuf)
  add_dependencies(protobuf SuperBuild::abseil-cpp SuperBuild::zlib)
  set_target_properties(protobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
  target_link_libraries(protobuf INTERFACE SuperBuild::libprotobuf SuperBuild::libprotobuf-lite SuperBuild::libprotoc SuperBuild::libutf8_range SuperBuild::libutf8_validity SuperBuild::abseil-cpp SuperBuild::zlib
  )
endif()

if(grpc IN_LIST projects)
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
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}address_sorting address_sorting "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}grpc grpc "")
  make_static_imported_library(${CMAKE_STATIC_LIBRARY_PREFIX}gpr gpr "")
  top_level_static_imported_library(grpc++ grpc++ "")
  add_dependencies(SuperBuild::grpc++ SuperBuild::grpc SuperBuild::gpr 
	SuperBuild::upb SuperBuild::address_sorting SuperBuild::zlib
    SuperBuild::protobuf SuperBuild::openssl SuperBuild::cares SuperBuild::re2
  )
  target_link_libraries(SuperBuild::grpc++ INTERFACE SuperBuild::grpc SuperBuild::gpr
    SuperBuild::upb SuperBuild::address_sorting SuperBuild::zlib
    SuperBuild::protobuf SuperBuild::openssl SuperBuild::cares SuperBuild::re2
  )
endif()
