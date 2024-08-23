#version 330 core

#define STEPS mod(iTime, 32.0)
#define STRIP_WIDTH 2.0

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D Texture0;
uniform sampler2D iChannel1;

in vec2 TexCoord;
out vec4 FragColor;

vec2 rotate(vec2 p, float angle) {
    return vec2(p.x * cos(angle) + p.y * sin(angle), p.y * cos(angle) - p.x * sin(angle));
}

vec3 shade(vec2 fragCoord, vec3 color) {
    float id = dot(color, vec3(0.299, 0.587, 0.114));
    id = floor(id * STEPS) / 100.0;
    
    float rotation = texture(iChannel1, vec2(id, 0.0)).r * 2.0 * 3.1415;
    fragCoord = rotate(fragCoord, rotation);
    
    float intensity = texture(iChannel1, vec2(0.0, id)).r;
    intensity = intensity * 0.75 + 0.25;
    intensity = mix(intensity - 0.1, intensity + 0.1, step(mod(fragCoord.x, STRIP_WIDTH * 2.0), STRIP_WIDTH));
 
    return vec3(0.5, 0.5, 1.0) * intensity;
}

float edge(vec2 fragCoord, vec2 resolution) {
    mat3 G[9];
    
    G[0] = 1.0 / (2.0 * sqrt(2.0)) * mat3(1.0, sqrt(2.0), 1.0, 0.0, 0.0, 0.0, -1.0, -sqrt(2.0), -1.0);
    G[1] = 1.0 / (2.0 * sqrt(2.0)) * mat3(1.0, 0.0, -1.0, sqrt(2.0), 0.0, -sqrt(2.0), 1.0, 0.0, -1.0);
    G[2] = 1.0 / (2.0 * sqrt(2.0)) * mat3(0.0, -1.0, sqrt(2.0), 1.0, 0.0, -1.0, -sqrt(2.0), 1.0, 0.0);
    G[3] = 1.0 / (2.0 * sqrt(2.0)) * mat3(sqrt(2.0), -1.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, -sqrt(2.0));
    G[4] = 1.0 / 2.0 * mat3(0.0, 1.0, 0.0, -1.0, 0.0, -1.0, 0.0, 1.0, 0.0);
    G[5] = 1.0 / 2.0 * mat3(-1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0);
    G[6] = 1.0 / 6.0 * mat3(1.0, -2.0, 1.0, -2.0, 4.0, -2.0, 1.0, -2.0, 1.0);
    G[7] = 1.0 / 6.0 * mat3(-2.0, 1.0, -2.0, 1.0, 4.0, 1.0, -2.0, 1.0, -2.0);
    G[8] = 1.0 / 3.0 * mat3(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    
    mat3 I;
    float cnv[9];
    vec3 sample_val;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sample_val = texture(Texture0, (vec2(fragCoord) + vec2(i - 1, j - 1)) / resolution).rgb;
            I[i][j] = length(sample_val); 
        }
    }
    
    for (int i = 0; i < 9; i++) {
        float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);
        cnv[i] = dp3 * dp3; 
    }

    float M = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]);
    float S = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + (cnv[8] + M); 
    
    return sqrt(M / S);
}

void main() {
    vec2 uv = TexCoord;
    vec3 color = texture(Texture0, uv).rgb;
    if (edge(gl_FragCoord.xy, iResolution) < 0.05) {
        color = shade(gl_FragCoord.xy, color);
    }
    
    FragColor = vec4(color, 1.0);
}
