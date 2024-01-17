uniform sampler2D texture;
uniform sampler2D background;

void main() {

    vec3 tex_color = texture2D(texture, gl_TexCoord[0].xy).rgb;
    vec3 b_color = texture2D(background, gl_TexCoord[0].xy).rgb;
    // vec3 color = vec3(1.,0.,0.);
    // vec3 color = b_color.rgb;
    vec3 a = vec3(1. - b_color.r , 1. - b_color.g, 1. - b_color.b);
    tex_color = smoothstep(0.1, 0.8, tex_color);
    // b_color = b_color - tex_color;
    // vec3 color = b_color;
    vec3 color = mix(b_color,a, tex_color);

    gl_FragColor = vec4(color,1.);
}