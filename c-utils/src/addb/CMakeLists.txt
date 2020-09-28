include_directories("/usr/include/motr")

SET(UTILS_ADDB_PLUGIN_SRCS
	plugin.c
)

add_library(utils-addb-plugin OBJECT ${UTILS_ADDB_PLUGIN_SRCS})