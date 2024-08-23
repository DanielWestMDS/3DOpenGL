#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D Texture0;
uniform sampler2D iChannel1;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    vec2 uv = TexCoord;
    float time = iTime;
    vec3 raintex = texture(iChannel1, vec2(uv.x * 2.0, uv.y * 0.1 + time * 0.125)).rgb / 8.0;
    vec2 where = (uv - raintex.xy);
    vec3 texchur1 = texture(Texture0, where).rgb;
    
    FragColor = vec4(texchur1, 1.0);
}
