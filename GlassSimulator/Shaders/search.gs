#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


flat out int trigId;
void main() {    

trigId = gl_PrimitiveIDIn;
gl_Position = gl_in[0].gl_Position;
EmitVertex();   
gl_Position = gl_in[1].gl_Position;
EmitVertex();
gl_Position = gl_in[2].gl_Position;
EmitVertex();
EmitVertex();
EndPrimitive();

}  


