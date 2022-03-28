#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the side-length
uniform float side_length;
uniform vec2 center;
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    // here is the equation for drawing the square , we got square's center & side_length
    // we get the max from the difference of (x_point - x_center) & (y_point - y_center)
    // and if that max is <= half the side_length of square that means it's located inside square boundaries
    if(max(abs(gl_FragCoord.x - center[0]) , abs(gl_FragCoord.y - center[1])) <= side_length/2){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}