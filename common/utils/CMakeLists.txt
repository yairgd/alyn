set(commnon_SRC
	./logger.c
	./system_model.c	
)

if (NOT  ${ZEPHYR})
	list(APPEND commnon_SRC "./ThreadPool.cpp")
endif()


add_library(common_static STATIC
	${commnon_SRC}
)

target_include_directories(common_static PUBLIC 
	.
	..
)
target_compile_options(common_static PRIVATE ${cpu_compile_flags})



# Get properties of target1
#get_target_properties(target1_properties app
#    PROPERTIES
#        COMPILE_DEFINITIONS
#        INCLUDE_DIRECTORIES
#        COMPILE_OPTIONS
#)

# Set properties for target2
#target_compile_definitions(target2 PRIVATE ${target1_properties.COMPILE_DEFINITIONS})
#target_include_directories(target2 PRIVATE ${target1_properties.INCLUDE_DIRECTORIES})
#target_compile_options(target2 PRIVATE ${target1_properties.COMPILE_OPTIONS})



#include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
#include(gcc/compiler_flags.cmake)

#message ("${COMMON_COMPILE_FLAGS}EEEE!!!!!!!!!!!!!!!!!!!!!!!!" )
# Use common compile flags
#target_compile_options(common_static PRIVATE ${COMMON_COMPILE_FLAGS})
