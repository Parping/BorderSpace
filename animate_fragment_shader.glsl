// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;
uniform float time;
uniform vec2 frame;
uniform int current_frame;
// Color modifier: we multiply each component of the color by each
// component of this modifier
uniform vec3 color_mod;

void main()
{
    // Sample texture
    

    vec2 uv = uv_interp;
    float speed=0.1;
    float index =mod(floor(time/speed),frame.x);
    uv.x=uv.x*(1.0/frame.x)+ index*(1.0/frame.x);
    
    vec4 color = texture2D(onetex, uv);

    // Assign color to fragment

    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
