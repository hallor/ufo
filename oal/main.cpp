#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alut.h>
#include <unistd.h>
#include <cmath>

#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[]={0.0,0.0,0.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat listenerOri[]={0.0,0.0,0.0, 0.0,1.0,0.0};

ALfloat source0Pos[]={ 0.0, 1.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

int main(int argc, char **argv)
{
  ALuint buffer, source;

  alutInit (NULL, NULL);

  alGenBuffers(1, &buffer);
  alGenSources (1, &source);

  {
    int fd = 0;
    if (argc < 2)
      fd = open("XCOMA/RAWSOUND/STRATEGC/VEHICLE/BIGUFO2.RAW", O_RDONLY);
    else
      fd = open(argv[1], O_RDONLY);

    struct stat x;
    fstat(fd, &x);
    int size = x.st_size;

    char * buf = (char*)calloc(size, 1);

    size = (size > 10000000 ? 10000000 : size);

    if (read(fd, buf, size)!=size)
    {
      printf("Error i chuj\n");
      return -1;
    }
    printf("Read %i bytes.\n", size);

    alBufferData(buffer, AL_FORMAT_MONO8, buf, size, 11025*2);
    free(buf);
  }

  alSourcei (source, AL_BUFFER, buffer);
  alListenerfv(AL_POSITION, listenerPos);
  alListenerfv(AL_ORIENTATION, listenerOri);
  alSourcei(source, AL_LOOPING, AL_TRUE);

  alSourcefv(source, AL_POSITION, source0Pos);

  alSourcePlay (source);

  int q=AL_PLAYING;

  float c  = 0, r = 2;
  while (q == AL_PLAYING)
  {
    alGetSourcei(source, AL_SOURCE_STATE, &q);
    source0Pos[0]= r *cos(c) - 0*sin(c);
    source0Pos[1]= r *sin(c) + 0*cos(c);
    alSourcefv(source, AL_POSITION, source0Pos);
    usleep(100000);
    c += 0.1;
  }

  alutExit ();

  return 0;
}
