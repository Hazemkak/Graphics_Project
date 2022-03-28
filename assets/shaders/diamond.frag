#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the side-length
uniform float side_length;
uniform vec2 center;
uniform vec4 inside_color ;
uniform vec4 outside_color ;

void main(){
    //TODO: Write code that will draw the diamond
    //Here we get the distance between the pixel to be colored and the center of diamond using the manhattan distance
    //and check if the point lies outside or inside the shape we want to draw .
    if(abs(gl_FragCoord.x - center[0]) + abs(gl_FragCoord.y - center[1]) <= side_length/2){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}