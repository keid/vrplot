// simple.frag

// back face coordinate
uniform sampler2D tex;
uniform sampler3D volume_tex;

varying vec4 pos;
varying vec3 pos_front;


void main (void)
{
  vec2 texc = ((pos.xy/pos.w) + vec2(1)) / vec2(2);
  vec4 pos_back = texture2D(tex, texc);

  vec3 dir;
  dir.x = pos_back.x - pos_front.x;
  dir.y = pos_back.y - pos_front.y;
  dir.z = pos_back.z - pos_front.z;

  float len_volume = length(dir.xyz);
  
  vec3 dir_norm = normalize(dir);
  
  vec3 current_pos = pos_front;
  vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

  vec4 color_volume;

  int IT_MAX = 256;
  float step = ( 1.0 / float(IT_MAX) );

  //for ( int i=0; i<IT_MAX; ++i ) {
  while( len_volume > 0.0 ) {
    color_volume = texture3D( volume_tex, current_pos );

    //color_volume.w *= 0.5;

    color_volume.xyz *= vec3(color_volume.w);
    
    color = vec4(1.0 - color.w) * color_volume + color;
    
    len_volume -= step;
    current_pos += vec3(step) * dir_norm;

    if ( color.w > 0.95 ) break;
    if ( len_volume < 0.0 ) break;
  }
    
  gl_FragColor = color;
  
}
