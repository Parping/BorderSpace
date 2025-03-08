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
vec3 rgb2gray(vec3 color) {//create gray vec3
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));//Gray = R*0.299 + G*0.587 + B*0.114_rgb
    return vec3(luminance);
}
void main()
{
    // Sample texture
    vec2 uv = uv_interp;
    
    float index=current_frame;
    float y_inde=0;
    if(frame.y==1){
        uv.x=uv.x*(1.0/frame.x)+index*(1.0/frame.x);
    }
    else{
        if(current_frame>=frame.x){
            index=mod(current_frame,frame.x);
            y_inde=floor(current_frame/frame.x);
            uv.x=uv.x*(1.0/frame.x)+index*(1.0/frame.x);
            uv.y=uv.y*(1.0/frame.y)+y_inde*(1.0/frame.y);
        }else{
        uv.x=uv.x*(1.0/frame.x)+index*(1.0/frame.x);
        uv.y=uv.y*(1.0/frame.y)+y_inde*(1.0/frame.y);
        }
    }

    
    
    vec4 color = texture2D(onetex, uv);
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
