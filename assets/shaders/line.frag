#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the slope and the intercept
uniform float slope;
uniform float intercept;
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the square
    // to get the line equation we need slope and y-intercept
    // line equation : y = slope*x + y-intercept
    // if the y <= slope*x + y-intercept so it's below the line otherwise it's above the line
    if(gl_FragCoord.y <= slope*gl_FragCoord.x + intercept){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}