// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;
uniform int ghost;
uniform vec2 frame;

uniform int from;

vec3 rgb2gray(vec3 color) {//create gray vec3
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));//Gray = R*0.299 + G*0.587 + B*0.114_rgb
    return vec3(luminance);
}
vec3 rgb2Red(vec3 color) {//create red vec3
    float luminance = dot(color, vec3(0.5, 0.1, 0.1));//Gray 
    return vec3(luminance);
}
void main()
{
    // Sample texture
    vec2 uv = uv_interp;


    vec4 color = texture2D(onetex, uv);
    // Assign color to fragment
    
    if(ghost==1){
        vec3 gray = rgb2gray(vec3(color.r,color.g,color.b));
        gl_FragColor = vec4(gray, color.a * 0.5);//transformation
    }
    else if(from==-2){
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }
    else if(from==91){
        gl_FragColor = vec4(0.0, 1.0, 0.0, color.a);
    }
    else if(from ==92){
        gl_FragColor = vec4(1.0, 1.0, 0.0, color.a);
    }
    else if(from == 94){
        gl_FragColor = vec4(1.0, 0.1, 0.1, color.a);
    }
    else{
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }
    

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
