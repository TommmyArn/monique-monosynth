
find_package(Git)

if (EXISTS ${MONIQUESRC}/VERSION_GIT_INFO)
    message(STATUS "VERSION_GIT_INFO file is present; using that rather than git query")
    # Line 2 is the branch, line 3 is the hash
    execute_process(
            COMMAND sed -n "2p" ${MONIQUESRC}/VERSION_GIT_INFO
            WORKING_DIRECTORY ${MONIQUESRC}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
            COMMAND sed -n "3p" ${MONIQUESRC}/VERSION_GIT_INFO
            WORKING_DIRECTORY ${MONIQUESRC}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

elseif (Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${MONIQUESRC}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${MONIQUESRC}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

if ("${GIT_BRANCH}" STREQUAL "")
    message(WARNING "Could not determine Git branch, using placeholder.")
    set(GIT_BRANCH "git-no-branch")
endif ()
if ("${GIT_COMMIT_HASH}" STREQUAL "")
    message(WARNING "Could not determine Git commit hash, using placeholder.")
    set(GIT_COMMIT_HASH "git-no-commit")
endif ()

if (WIN32)
    set(MONIQUE_BUILD_ARCH "x86")
else ()
    execute_process(
            COMMAND uname -m
            OUTPUT_VARIABLE MONIQUE_BUILD_ARCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

cmake_host_system_information(RESULT MONIQUE_BUILD_FQDN QUERY FQDN)

message(STATUS "Setting up surge version")
message(STATUS "  git hash is ${GIT_COMMIT_HASH} and branch is ${GIT_BRANCH}")
message(STATUS "  buildhost is ${MONIQUE_BUILD_FQDN}")
message(STATUS "  buildarch is ${MONIQUE_BUILD_ARCH}")

if (${AZURE_PIPELINE})
    message(STATUS "Azure Pipeline Build")
    set(lpipeline "pipeline")
else ()
    message(STATUS "Developer Local Build")
    set(lpipeline "local")
endif ()

if (${GIT_BRANCH} STREQUAL "main")
    if (${AZURE_PIPELINE})
        set(lverpatch "nightly")
    else ()
        set(lverpatch "main")
    endif ()
    set(lverrel "999")
    set(fverpatch ${lverpatch})
else ()
    string(FIND ${GIT_BRANCH} "release/" RLOC)
    if (${RLOC} EQUAL 0)
        message(STATUS "Configuring a Release Build from '${GIT_BRANCH}'")
        string(SUBSTRING ${GIT_BRANCH} 13 100 RV) # that's release-xt slash 1.7.
        string(FIND ${RV} "." DLOC)
        if (NOT (DLOC EQUAL -1))
            math(EXPR DLP1 "${DLOC} + 1")
            string(SUBSTRING ${RV} ${DLP1} 100 LRV) # skip that first dots
            set(lverrel ${LRV})
        else ()
            set(lverrel "99")
        endif ()
        set(lverpatch "stable-${lverrel}")
        set(fverpatch "${lverrel}")
    else ()
        set(lverpatch ${GIT_BRANCH})
        set(fverpatch ${lverpatch})
        set(lverrel "1000")
    endif ()
endif ()

set(MONIQUE_FULL_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${fverpatch}.${GIT_COMMIT_HASH}")
set(MONIQUE_MAJOR_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}")
set(MONIQUE_SUB_VERSION "${CMAKE_PROJECT_VERSION_MINOR}")
set(MONIQUE_RELEASE_VERSION "${lverpatch}")
set(MONIQUE_RELEASE_NUMBER "${lverrel}")
set(MONIQUE_BUILD_HASH "${GIT_COMMIT_HASH}")
set(MONIQUE_BUILD_LOCATION "${lpipeline}")

string(TIMESTAMP MONIQUE_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP MONIQUE_BUILD_YEAR "%Y")
string(TIMESTAMP MONIQUE_BUILD_TIME "%H:%M:%S")

message(STATUS "Using MONIQUE_VERSION=${MONIQUE_FULL_VERSION}")

message(STATUS "Configuring ${MONIQUEBLD}/geninclude/version.cpp")
configure_file(${MONIQUESRC}/Source/version.cpp.in
        ${MONIQUEBLD}/geninclude/version.cpp)

