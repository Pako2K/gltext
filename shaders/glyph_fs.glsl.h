const char __FRAG_SHADER_SRC__ [] =
" \
#version 400 core \n\
\
in vec2 vTexCoords; \
\
out vec4 fColor; \
\
uniform sampler2D text; \
uniform vec3 uTextColor; \
\
void main() { \
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vTexCoords).r); \
\
  fColor = vec4(uTextColor, 1.0) * sampled;  \
} \
";
