// gouraud.vert

varying vec3 pos;
varying vec3 dir;

void main(void)
{
  vec3 position = vec3(gl_ModelViewMatrix * gl_Vertex);
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec3 light = normalize(gl_LightSource[0].position.xyz - position);
  float diffuse = dot(light, normal);
	
  //gl_FrontColor = gl_FrontLightProduct[0].ambient;

  pos = vec3( normalize(gl_Vertex) * 0.5 + 0.5 );
  dir = vec3( vec4(position, 1.0) * gl_ModelViewMatrixInverse );
  //norm = vec3( gl_ModelViewMatrix * vec4( gl_Normal, 1.0) );
  //norm = normal;
  //norm = gl_Normal;
  
  //gl_FrontColor = gl_Vertex;
  gl_FrontColor = vec4( pos, 1.0 );
 
  //gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
  gl_Position = ftransform();
}
