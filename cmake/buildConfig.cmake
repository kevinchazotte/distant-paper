include("${CMAKE_CURRENT_LIST_DIR}/projects.cmake")

set(CMAKE_PREFIX_PATH "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}")

if (UNIX)
  find_package(PkgConfig REQUIRED)
endif()

function(create_static_imported_library target_name libfile_name debug_suffix)
  add_library(SuperBuild::${target_name} STATIC IMPORTED GLOBAL)

  # explicitly set imported locations per config that may be generated for the library
  set_target_properties(SuperBuild::${target_name} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${PROJECT_BUILD_DIR}/x64/Debug/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${libfile_name}${debug_suffix}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${PROJECT_BUILD_DIR}/x64/RelWithDebInfo/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION_RELEASE "${PROJECT_BUILD_DIR}/x64/Release/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${libfile_name}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>/include/"
  )
endfunction()

# create library targets for each added third party project

# SFML has different Mac/UNIX behavior AND different lib files for static linking and debug configs
# see https://www.sfml-dev.org/tutorials/2.5/start-vc.php
if("SFML" IN_LIST projects)
  if(WIN32)
    create_static_imported_library(SFML sfml-main "-d")
    set_target_properties(SuperBuild::SFML PROPERTIES
      INTERFACE_COMPILE_DEFINITIONS SFML_STATIC
    )
    # to statically link SFML, you need to manually link all dependencies, which are included library files in SFML
    set(SFML_COMPONENTS graphics window audio network system)
    set(SFML_DEPENDENCIES flac freetype ogg vorbis vorbisenc vorbisfile)
    set(SFML_SYSTEM_LIBS ws2_32.lib winmm.lib opengl32.lib)
    foreach(component IN LISTS SFML_COMPONENTS)
      target_link_libraries(SuperBuild::SFML INTERFACE "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-${component}-s$<$<CONFIG:Debug>:-d>${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endforeach()
    foreach(dependency IN LISTS SFML_DEPENDENCIES)
      target_link_libraries(SuperBuild::SFML INTERFACE "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${dependency}$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}")
     endforeach()
    target_link_libraries(SuperBuild::SFML INTERFACE ${SFML_SYSTEM_LIBS})
  elseif(UNIX)
    add_library(SFML INTERFACE) # on linux, there is no libsfml-main.a file
    add_library(SuperBuild::SFML ALIAS SFML)
    set_target_properties(SFML PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/include/"
      INTERFACE_COMPILE_DEFINITIONS SFML_STATIC
    )
    # to statically link SFML, you need to manually link all dependencies, which are included library files in SFML
    set(SFML_COMPONENTS graphics window audio network system)
    set(REQUIRED_PACKAGES x11 xrandr xcursor xi freetype2 libudev)
    foreach(component IN LISTS SFML_COMPONENTS)
      target_link_libraries(SFML INTERFACE "${PROJECT_BUILD_DIR}/x64/${CMAKE_BUILD_TYPE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-${component}-s$<$<CONFIG:Debug>:-d>${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endforeach()
    foreach(package IN LISTS REQUIRED_PACKAGES)
      string(TOUPPER ${package} package_var)
      pkg_check_modules(${package_var} REQUIRED ${package})
    endforeach()
    target_link_libraries(SFML INTERFACE ${X11_STATIC_LIBRARIES} ${XRANDR_STATIC_LIBRARIES} ${XCURSOR_STATIC_LIBRARIES} ${XI_STATIC_LIBRARIES} ${FREETYPE2_STATIC_LIBRARIES} ${LIBUDEV_STATIC_LIBRARIES})
  endif()
endif()

if(zlib IN_LIST projects)
  if(WIN32)
    create_static_imported_library(zlib zlibstatic "d")
  elseif(UNIX)
    create_static_imported_library(zlib z "")
  endif()
endif()

if(protobuf IN_LIST projects)
  find_package(absl)
  # all protobuf lib files have prefix "lib"
  set(UTF8_LIBRARIES utf8_range utf8_validity)
  set(PROTOBUF_LIBRARIES protobuf protobuf-lite protoc)
  foreach(utf8_lib IN LISTS UTF8_LIBRARIES)
    if(WIN32)
      create_static_imported_library(${utf8_lib} lib${utf8_lib} "")
    elseif(UNIX)
      create_static_imported_library(${utf8_lib} ${utf8_lib} "")
    endif()
  endforeach()
  foreach(protobuf_lib IN LISTS PROTOBUF_LIBRARIES)
    if(WIN32)
      create_static_imported_library(${protobuf_lib} lib${protobuf_lib} "d")
    elseif(UNIX)
      create_static_imported_library(${protobuf_lib} ${protobuf_lib} "d")
    endif()
  endforeach()
  
  set(ABSL_DEPS absl::absl_check;absl::absl_log;absl::algorithm;absl::base;absl::bind_front;absl::bits;absl::btree;absl::cleanup;absl::cord;absl::core_headers;absl::debugging;absl::die_if_null;absl::dynamic_annotations;absl::flags;absl::flat_hash_map;absl::flat_hash_set;absl::function_ref;absl::hash;absl::layout;absl::log_initialize;absl::log_globals;absl::log_severity;absl::memory;absl::node_hash_map;absl::node_hash_set;absl::optional;absl::random_distributions;absl::random_random;absl::span;absl::status;absl::statusor;absl::strings;absl::synchronization;absl::time;absl::type_traits;absl::utility;absl::variant)

  set_target_properties(SuperBuild::protobuf-lite PROPERTIES INTERFACE_LINK_LIBRARIES "${ABSL_DEPS};\$<LINK_ONLY:SuperBuild::utf8_validity>")

  set_target_properties(SuperBuild::protoc PROPERTIES INTERFACE_LINK_LIBRARIES "\$<LINK_ONLY:SuperBuild::protobuf>;${ABSL_DEPS}")

  set_target_properties(SuperBuild::protobuf PROPERTIES INTERFACE_LINK_LIBRARIES ${ABSL_DEPS})
  target_link_libraries(SuperBuild::protobuf INTERFACE SuperBuild::zlib SuperBuild::utf8_range SuperBuild::utf8_validity)

  add_dependencies(SuperBuild::protobuf SuperBuild::zlib)
endif()

if(boringssl IN_LIST projects)
  create_static_imported_library(crypto crypto "")
  create_static_imported_library(openssl ssl "")
  add_dependencies(SuperBuild::openssl SuperBuild::crypto)
  target_link_libraries(SuperBuild::openssl INTERFACE SuperBuild::crypto)
endif()

if(cares IN_LIST projects)
  create_static_imported_library(cares cares "")
endif()

if(re2 IN_LIST projects)
  create_static_imported_library(re2 re2 "")
endif()

if(grpc IN_LIST projects)
  find_package(absl)

  add_library(upb INTERFACE)
  add_library(SuperBuild::upb ALIAS upb)
  set(UPB_LIBRARIES upb_textformat_lib upb_wire_lib upb_message_lib upb_mini_descriptor_lib upb_json_lib upb_mem_lib upb_base_lib)
  foreach(upb_lib IN LISTS UPB_LIBRARIES)
    create_static_imported_library(${upb_lib} ${upb_lib} "")
    target_link_libraries(upb INTERFACE SuperBuild::${upb_lib})
  endforeach()

  create_static_imported_library(address_sorting address_sorting "")
  create_static_imported_library(gpr gpr "")
  create_static_imported_library(grpc grpc "")
  create_static_imported_library(grpc++ grpc++ "")

  set(GPR_ABSL_DEPS absl::base;absl::core_headers;absl::log_severity;absl::cleanup;absl::flags;absl::flags_marshalling;absl::any_invocable;absl::check;absl::log_globals;absl::log;absl::memory;absl::bits;absl::random_random;absl::status;absl::cord;absl::str_format;absl::strings;absl::synchronization;absl::time;absl::optional)

  set(GPRC_ABSL_DEPS absl::algorithm_container;absl::config;absl::no_destructor;absl::flat_hash_map;absl::flat_hash_set;absl::inlined_vector;absl::bind_front;absl::function_ref;absl::hash;absl::type_traits;absl::random_bit_gen_ref;absl::random_distributions;absl::statusor;absl::span;absl::utility)

  set_target_properties(SuperBuild::gpr PROPERTIES INTERFACE_LINK_LIBRARIES "${GPR_ABSL_DEPS}")

  set_target_properties(SuperBuild::grpc PROPERTIES INTERFACE_LINK_LIBRARIES "SuperBuild::upb;SuperBuild::re2;SuperBuild::zlib;${GPRC_ABSL_DEPS};SuperBuild::cares;SuperBuild::gpr;SuperBuild::openssl;SuperBuild::address_sorting")

  set_target_properties(SuperBuild::grpc++ PROPERTIES INTERFACE_LINK_LIBRARIES "absl::absl_check;absl::absl_log")
  target_link_libraries(SuperBuild::grpc++ INTERFACE SuperBuild::grpc SuperBuild::protobuf)

  add_dependencies(SuperBuild::grpc++ SuperBuild::grpc SuperBuild::gpr SuperBuild::upb SuperBuild::address_sorting SuperBuild::zlib SuperBuild::protobuf SuperBuild::openssl SuperBuild::cares SuperBuild::re2)
endif()

