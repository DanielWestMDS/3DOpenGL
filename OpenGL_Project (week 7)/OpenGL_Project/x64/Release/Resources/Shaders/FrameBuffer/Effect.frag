#version 330 core

#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D Texture0;

// Input from vertex shader
in vec2 TexCoord;
out vec4 FragColor;

float getD(float angle, float radius, vec2 st) {
    float d = cos(floor(.5 + angle / radius) * radius - angle) * length(st);
    return d;
}

vec4 gr(vec2 g) {
    g *= 0.1;
    float color = sign((mod(g.x, 0.1) - 0.05) * (mod(g.y, 0.5) - 0.05));
    return sqrt(vec4(color));
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution - vec2(0.5, 0.5);
    float t = iTime * 2.0;
    float a = atan(uv.x, uv.y);
    float d = max(max(abs(uv.y), abs(uv.x)), min((uv.x), length(uv)));

    vec2 st = (gl_FragCoord.xy * 2.0 - iResolution) / min(iResolution.x, iResolution.y);

    vec3 color = vec3(0.0);
    float d3 = 0.0;

    float N = 9.0;

    float angle = atan(st.x, st.y) + PI * 0.25 - iTime / 3.0;
    float angle2 = atan(st.x, st.y) + PI * 0.25 + iTime / 3.0;

    float col = 1.0;
    int segno = 1;
    float stepRadius = 0.25;
    int count = 0;
    float currAngle = angle;
    float direction = 2.0;
    for (int i = 0; i < 15; i++) {
        float radius = TWO_PI / float(N);
        col -= float(segno) * smoothstep(stepRadius, stepRadius + 0.02 * (sin(16.0 * iTime) + 1.5), getD(currAngle, radius, st));
        count += 1;
        segno = segno == 1 ? -1 : 1;
        if (count == 2) {
            count = 0;
            currAngle = currAngle == angle ? angle2 : angle;
            N -= 3.0 * direction;
            if (N <= 0.0) {
                N = 3.0;
                direction = direction * -1.0;
            }
        }
        stepRadius -= tan(0.0446 * cos(iTime));
    }
    color = vec3(col);
    vec4 col3 = gr(2.0 * vec2(a, 0.8 / d + t * 0.9));
    vec2 p = (gl_FragCoord.xy - 0.5 * iResolution) / min(iResolution.x, iResolution.y);
    vec3 c = vec3(0.005, 0.0021, 0.009); // BG color
    vec3 lightCol = vec3(0.2, 0.43, 0.8);

    for (int i = 0; i < 10; ++i) {
        float t = 0.8 * 3.14 * float(i) / 5.0 + iTime;
        float x = cos(t) * 0.4;
        float y = sin(t);
        vec2 o = 0.15 * vec2(x - y, y);
        vec2 r = 0.15 * vec2(x + y, y);
        vec2 s = 0.15 * vec2(y * 1.5, x * 0.6);
        c += 0.0012 / (length(p - (o))) * lightCol * 0.961;
        c += 0.01 / (length(p - (r))) * lightCol * 0.5;
        c += 0.02 / (length(p - (s))) * lightCol * 0.5;
    }
    FragColor = col3 * vec4(0.2, 1.2, 1.0, 1.0);
    FragColor *= vec4(color * c, 1.0);
    FragColor += vec4(c * c, 1.0);
}
