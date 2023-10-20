set(commnon_SRC
	./logger.c
	./system_model.c

)

if (NOT DEFINED ZEPHYR)
	list(APPEND commnon_SRC "./timing.c")
endif()

add_library(common_static STATIC
	${commnon_SRC}
)

target_include_directories(common_static PUBLIC 
	.
)


