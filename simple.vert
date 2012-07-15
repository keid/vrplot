// simple.vert

varying vec4 pos;
varying vec3 pos_front;

void main(void)
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_FrontColor = gl_Color;
  pos_front = vec3(gl_Color);
  pos = gl_Position;
}
