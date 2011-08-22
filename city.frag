/* Fragment shader */
uniform vec2 mousePos;
//uniform sampler2D tex;
varying vec2 vTexCoord;

void main()
{
  vec4 col = texture2D(texUnit0, vTexCoord);
//  if (col.a < 0.5)
//    discard;
//  vec4 mouseCol = vec4(1,1,1,1);
//  float dist = distance(gl_FragCoord, mousePos);

//  if (dist > 2000)
    gl_FragColor = vec4(1,1,1,1);
//  else
//    gl_FragColor = col ;
}

