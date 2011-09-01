#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alut.h>
#include <unistd.h>

#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
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
      fd = open("XCOMA/RAWSOUND/ZEXTRA/ALERT.RAW", O_RDONLY);
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

  alSourcePlay (source);

  int q=AL_PLAYING;

  while (q == AL_PLAYING)
  {
    alGetSourcei(source, AL_SOURCE_STATE, &q);
    usleep(100000);
  }

  alutExit ();

  return 0;
}
