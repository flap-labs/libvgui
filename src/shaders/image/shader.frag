#version 330 core

in vec2 textureCoords;
out vec4 fragColor;

uniform sampler2D texture0;

void main() {
  vec4 textureColor = texture(texture0, textureCoords);
  
  if(textureColor.a < 0.1)
    discard;

  fragColor = textureColor;
}
