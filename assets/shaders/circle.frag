#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the radius
uniform float radius ;
uniform vec2 center ;
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the circle

    /*
    this condition will make the circle to be drown as the distance between the center 
    and the point coordinates will be calculated and if the it is larger than the redius 
    this mean it is outside the shape otherwise it is inside the shape . 
    */
    float eucilidean = sqrt(((gl_FragCoord.x - center[0]) * (gl_FragCoord.x - center[0])) + ((gl_FragCoord.y - center[1]) * (gl_FragCoord.y - center[1])));
    
    if(eucilidean <= radius){
        frag_color = inside_color;
    }else{
        frag_color = outside_color;
    }
}