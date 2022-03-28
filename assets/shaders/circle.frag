#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the radius
uniform float radius ;
uniform vec2 center ;
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the circle
    float eucilidean = 
        sqrt(((gl_FragCoord.x - center[0]) * (gl_FragCoord.x - center[0])) + ((gl_FragCoord.y - center[1]) * (gl_FragCoord.y - center[1])));
    // float x_xc=(gl_FragCoord.x-center[0])*(gl_FragCoord.x-center[0]);
    // float y_yc=(gl_FragCoord.y-center[1])*(gl_FragCoord.y-center[1]);
    // float r_power=radius*radius;
    if(eucilidean <= radius){
        frag_color = inside_color;
    }else{
        frag_color = outside_color;
    }
    // if(gl_FragCoord.x <= 256){
    //     frag_color = inside_color;
    // } else {
    //     frag_color = outside_color;
    // }
}