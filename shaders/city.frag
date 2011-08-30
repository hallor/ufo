/* Fragment shader */
uniform sampler2D tex;
uniform vec2 mousePos;
varying float zd;

void main()
{
  vec4 col = texture2D(tex, gl_TexCoord[0].st);


  if (col.a < 0.5)
    discard;

//  gl_FragColor = col;
//  return;

  vec2 mp = vec2(mousePos.x, -mousePos.y);
  float d = 1;//distance(mousePos, gl_FragCoord.xy) / 100.0;

  d = clamp(d, 0.0, 1.0);
  if (zd > 9.0/20.0)
    d = 1.0;

  gl_FragColor = col * d + vec4(1.0, 1.0, 0.0, 1.0-d) * (1.0 - d);
}

