#include <glm/detail/type_vec.hpp>

const glm::vec3 backgroundColor(0.04, 0.04, 0.04);
const glm::vec3 gridColor(0.25, 0.25, 0.25);

const char* primitiveVertexShader = R"glsl(
# version 330 core
layout (location = 0) in vec2 position;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(position, 0, 1);
}
)glsl";



const char* primitiveFragmentShader = R"glsl(
# version 330 core
uniform vec3 inColor;
out vec4 color;
void main() {
    color = vec4(inColor, 1);
}
)glsl";


const char* velementShader = R"glsl(
# version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
out vec2 texCoordFragment;
uniform vec2 shift;
uniform vec2 scale = vec2(1, 1);
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(scale * position + shift, 0, 1);
    texCoordFragment = texCoord;
}
)glsl";


const char* fragmentShader = R"glsl(
# version 330 core
in vec2 texCoordFragment;
out vec4 color;
uniform sampler2D sampler;
uniform vec3 mixColor;
uniform float mixCoeff = 0;
uniform float alphaMultiplier = 1;
void main() {
    color = mix(texture(sampler, texCoordFragment), vec4(mixColor, 1), mixCoeff);
    color.a *= alphaMultiplier;
}
)glsl";


const char* vertexShader = R"glsl(
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
out vec2 texCoordFragment;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(position, 0, 1);
    texCoordFragment = texCoord;
}
)glsl";


const char* glyphShader = R"glsl(
#version 330 core
in vec2 texCoordFragment;
out vec4 color;
uniform vec3 textColor;
uniform sampler2D glyph;
void main() {
    float alpha = texture(glyph, texCoordFragment).r;
    color = vec4(textColor, alpha);
}
)glsl";

