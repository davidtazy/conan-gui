# test ###################################################
find_package(Catch2 REQUIRED)

function(conangui_add_test)

  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs SRC LIBS COV)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}"
                        ${ARGN})

  add_executable(${ARG_TARGET} ${ARG_SRC})
  target_compile_features(${ARG_TARGET} PUBLIC cxx_std_17)

  target_compile_definitions(
    ${ARG_TARGET} PUBLIC UNITEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")

  target_link_libraries(${ARG_TARGET} PRIVATE ${ARG_LIBS} Catch2::Catch2)

  # test coverage with kcov and ctest ######################

  find_program(KCOV kcov)
  message("kcov: ${KCOV}")
  if(KCOV)
    enable_testing()

    set(include_args)
    foreach(cov_dir ${ARG_COV})
      # string(APPEND include_args " ")
      string(APPEND include_args "${CMAKE_CURRENT_LIST_DIR}/${cov_dir},")
    endforeach()

    # string(SUBSTRING include_args 0 -1 include_args) message(FATAL_ERROR "****
    # ${include_args}")

    add_test(
      NAME ${ARG_TARGET}_coverage
      COMMAND
        ${KCOV} --exclude-pattern=/usr/,.conan/data/
        --include-pattern=${include_args} ${CMAKE_BINARY_DIR}/cov
        $<TARGET_FILE:${ARG_TARGET}>)
  endif()

endfunction()
