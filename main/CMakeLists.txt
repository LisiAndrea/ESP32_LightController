set(includedirs
  "."
  "../include/"
  )


idf_component_register(SRCS "main.c" "net.c" "mqtt.c" "json.c"
                    INCLUDE_DIRS  ${includedirs})