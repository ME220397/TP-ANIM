varying vec4 uv;
uniform sampler2D texture;
uniform vec4 color;
uniform vec2 offset;
uniform float number_of_rows;

void main() {
    gl_FragColor = texture2D(texture, (uv.st/number_of_rows) + offset) * color;
}
