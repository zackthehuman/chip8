set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( TEST_BASE_DIR "${PROJECT_SOURCE_DIR}/test" )
set( EMULATOR_BASE_DIR "${PROJECT_SOURCE_DIR}" )

set( INCLUDE_DIRS
    ${TEST_BASE_DIR}/include
    ${EMULATOR_BASE_DIR}/include
)

set( REQUIRE_EMULATOR_SOURCE_FILES
    ${EMULATOR_BASE_DIR}/src/chip8/Functions.cpp
    ${EMULATOR_BASE_DIR}/src/chip8/Opcodes.cpp
)

set( TEST_SOURCE_FILES
    ${REQUIRE_EMULATOR_SOURCE_FILES}
    src/Main.cpp
    src/TestFunctions.cpp
    src/TestOpcodes.cpp
)

include_directories( ${INCLUDE_DIRS} )

add_executable( chip8-test ${TEST_SOURCE_FILES} ${INCLUDE_DIRS} )
