/* Vertex shader */
varying vec2 vTexCoord;

void main(void)
{
  vTexCoord = gl_MultiTexCoord0.xy;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
