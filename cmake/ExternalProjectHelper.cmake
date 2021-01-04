find_package(Git QUIET)

if(${GIT_FOUND} AND EXISTS ${CMAKE_SOURCE_DIR}/.git)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} --git-dir=.git describe --all
    RESULT_VARIABLE NOT_A_GIT_REPO
    ERROR_QUIET
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
else()
  set(NOT_A_GIT_REPO "NotAGitRepo")
endif()

set(THIRD_PARTY_DIRECTORY
    "${PROJECT_SOURCE_DIR}/external"
    CACHE PATH "The path were all the 3rd party projects can be found")

# initialize submodule with given path
function(initialize_submodule path)
  if(NOT ${GIT_FOUND})
    message(
      FATAL_ERROR "git not found and ${path} sub-module not cloned (use git clone --recursive)")
  endif()
  message(STATUS "Sub-module : missing ${path} : running git submodule update --init --recursive")
  execute_process(
    COMMAND
      ${GIT_EXECUTABLE}  submodule update --init --recursive -- ${path}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endfunction()

# check for external project and initialize submodule if it is missing
function(add_external_project name)
  find_path(
    ${name}_PATH
    NAMES CMakeLists.txt
    PATHS "${THIRD_PARTY_DIRECTORY}/${name}")
  if(NOT EXISTS ${${name}_PATH})
    if(NOT_A_GIT_REPO)
      message(FATAL_ERROR "Looks like you are building from source. Git needed for ${name} feature.")
    endif()
    initialize_submodule("${THIRD_PARTY_DIRECTORY}/${name}")
  else()
    message(STATUS "Sub-project : using ${name} from from external/${name}")
  endif()
  add_subdirectory("${THIRD_PARTY_DIRECTORY}/${name}")
endfunction()
