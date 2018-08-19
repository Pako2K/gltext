const char __VERT_SHADER_SRC__ [] =
" \
 \n\
#version 400 core \n\
 \n\
in ivec2 inPosition; \n\
in vec2 inTexCoord; \n\
 \n\
out vec2 vTexCoords; \n\
 \n\
uniform mat4 uProjection; \n\
 \n\
void main() { \n\
  gl_Position = uProjection * vec4(inPosition, 1.0, 1.0); \n\
  vTexCoords = inTexCoord; \n\
} \n\
 \n\
";
