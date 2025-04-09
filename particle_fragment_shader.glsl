// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;


// Texture sampler
uniform sampler2D onetex;
uniform int acc;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);
    if(acc==0){
    color.rgb = vec3(0.8, 0.4, 0.01) * color_interp.r;
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }
    else if(acc==1){
    color.rgb = vec3(0.01, 0.4, 0.8) * color_interp.r;
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }
    else{
    color.rgb = vec3(0.4, 0.8, 0.01) * color_interp.r;
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }


    // Assign color to fragment
    
    
    // Check for transparency
    if(color.a < 0.05)
    {
         discard;
    }
}
