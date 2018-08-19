const char __FRAG_SHADER_SRC__ [] =
" \
 \n\
#version 400 core \n\
 \n\
in vec2 vTexCoords; \n\
 \n\
out vec4 fColor; \n\
 \n\
uniform sampler2D text; \n\
uniform vec3 uTextColor; \n\
 \n\
 \n\
void main() { \n\
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vTexCoords).r); \n\
 \n\
	 \n\
  fColor = vec4(uTextColor, 1.0) * sampled; \n\
} \n\
 \n\
";
