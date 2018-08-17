const char __VERT_SHADER_SRC__ [] =
" \
#version 400 core \n\
\
in ivec2 inPosition; \
in vec2 inTexCoord; \
\
out vec2 vTexCoords; \
\
uniform mat4 uProjection; \
\
void main() { \
    gl_Position = uProjection * vec4(inPosition, 1.0, 1.0); \
    vTexCoords = inTexCoord; \
} \
";
