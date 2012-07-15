// gouraud.frag

varying vec3 pos;
varying vec3 dir;

void main (void)
{
  vec3 c_pos = pos; // current position

  vec4 color = vec4(0.0, 0.0, 0.0, 1.0); //gl_Color;

  vec3 c_dir = (1.0/256.0) * normalize( vec3(-dir.x, -dir.y, dir.z) );
  
  /*
  do{
    
    c_pos += c_dir;

    //color.x += c_dir.x;
    //color.y += c_dir.y;
    //color.z += c_dir.z;
    color.x += (1.0/256.0);
    

  } while( (c_pos.x >= 0.0) && (c_pos.x <= 1.0) 
	   && (c_pos.y >= 0.0) && (c_pos.y <= 1.0)
	   && (c_pos.z >= 0.0) && (c_pos.z <= 1.0));
*/

  gl_FragColor = color;
  gl_FragColor = vec4( pos, 1.0 );
  //gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
  //gl_FragColor = vec4( normalize(c_dir) * 0.5 + 0.5 , 1.0 );
  //gl_FragColor = vec4( norm, 1.0 );
  //gl_FragColor = vec4( norm.x * 0.5 + 0.5, 0.0, 0.0, 1.0 );
  
}
