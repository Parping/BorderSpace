// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;
uniform int ghost;
uniform vec2 frame;
uniform int current_frame;
uniform float percent;
uniform int offset;
vec3 rgb2gray(vec3 color) {//create gray vec3
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));//Gray = R*0.299 + G*0.587 + B*0.114_rgb
    return vec3(luminance);
}
void main()
{
    // Sample texture
    vec2 uv = uv_interp;
    vec4 color;
    int index=current_frame;

    

        vec2 empty_uv;
        empty_uv.y=uv.y;
        empty_uv.x=uv.x*(1.0/frame.x)+1*(1.0/frame.x);
        vec2 full_uv;
        full_uv.y=uv.y;
        full_uv.x=uv.x*(1.0/frame.x);

        if(index==1){
             color = texture2D(onetex, empty_uv);
        }else{
            if(uv.x <= percent){
             color = texture2D(onetex, full_uv);
             }
        }

    


    
    // Assign color to fragment
    
    if(ghost==1){
        vec3 gray = rgb2gray(vec3(color.r,color.g,color.b));
        gl_FragColor = vec4(gray, color.a * 0.5);//transformation
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
