#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include "gfx.h"
#include "citymap.h"
#include "cityitem.h"
#include "timer.h"
#include "mouse.h"
#include "screen.h"
#include "utils.h"
#include "shaderprogram.h"
#include "logger.h"
#include "os/FileIO.h"
#include "importer/cpngfile.h"
#include "importer/pckfile.h"
#include "importer/pcxfile.h"
#include "importer/tabfile.h"
#include "importer/bitmap.h"
#include "importer/cursor.h"

#include "citybuilding.h"
#include "importmanager.h"
#include "ResourceBase.h"
#include "SoundBufferManager.h"
#include "SoundBuffer.h"
#include "OpenAL.h"
#include "Property.h"
#include "AppSettings.h"

#include "Application.h"

using namespace std;

int main( int argc, char* argv[] )
{
//	Application * app = new Application();

//	return app->execute(argc, argv);
    OpenAL::Get().Initialize();
    cSoundBufferManager man;
    cSoundBuffer *buf = man.Get();
    buf->Release();
    OpenAL::Free();

  iFile *file = CreateFileIO();
  if (file)
  {
    file->Open("duapa.txt", FFileOpenFlags::OpenExisting | FFileOpenFlags::Read);
    char str[18] = {0};
    file->Read(str, sizeof(str));
    file->Close();
    ReleaseFileIO(file);
  }
  
  CityBuildings cb;
  ifstream bn("resources/building.nam");
  std::vector<std::string> names;
  names.reserve(150);
  while (bn.good())
  {
      std::string x;
      getline(bn, x);
      //bn >> x;
      names.push_back(x);
      LogInfo("Loaded name %s", x.c_str());
  }


  cb.load("XCOMA/UFODATA/CITYMAP1.BLD", names);

	ImportManager * im = CreateImportManager("XCOMA");
  GfxManager gfx;
  srand(42);
  //Start

	if (!im)
		return -1;

    Screen * screen = new Screen(AppSettings::GetWindowWidth(), AppSettings::GetWindowHeight(), "X-Com 42");

  {
    int ret;
    if ( (ret = glewInit())!= GLEW_OK)
    {
      cout << "Error initializing glew: " << glewGetErrorString(ret) << endl;
      return false;
    }
  }

  ShaderProgram sp("shaders/city.vert", "shaders/city.frag");

  if (!sp.compile())
  {
    cout << "Error compiling shader program." << endl;
    return -1;
  }

  if (!sp.link())
  {
    cout << "Error linking shader program."<< endl;
    return -1;
  }


  Raster * menu = new Raster(); //gfx.getRaster("XCOMA/UFODATA/BUYBASE2.PCX");

  {
		ResourceID x = im->getResourceID("UFODATA/BUYBASE2.PCX");
		const Surface *s = im->getSurface(x);
		menu->setSurface(*s); //TODO: refactor later somehow
  }
  menu->setZoom(2);

  SpritePack *city = gfx.getPack("XCOMA/UFODATA/CITY");

  SpritePack *ptang = gfx.getPack("XCOMA/UFODATA/PTANG");

  SpritePack *ufos = gfx.getPack("XCOMA/UFODATA/SAUCER");

  SpritePack *invalid = gfx.getPack("XCOMA/UFODATA/DESCURS");

  SpritePack *pequip = gfx.getPack("XCOMA/UFODATA/PEQUIP");

  cCursor mouse_cursors;

  {
  std::ifstream f("XCOMA/TACDATA/MOUSE.DAT", std::ios_base::binary);
  std::ifstream file_pal("screenshots/ufo010.pal", std::ios_base::binary);
  cPalette pal;
  LogInfo("Loading mouse cursors");

  pal.loadFrom(file_pal);

  mouse_cursors.loadFrom(f, pal);
  }

  Raster * mouse_img = new Raster();
  mouse_img->setSurface(*mouse_cursors.getSurface());

  CityMap cm(100,100,10);

  LogInfo("Loading city map...");

  bool res;

  if (argc <2)
    res = cm.load("XCOMA/UFODATA/CITYMAP1");
  else
    res = cm.load(argv[1]);

  cout << res << std::endl;

  Rect s;
  Rect camera;

  s.x = s.y = 0;
  camera.x = 0;
  camera.y = 0;
  camera.w = AppSettings::GetWindowWidth();
  camera.h = (Uint16)(AppSettings::GetWindowHeight() * 0.76f);

  bool quit = false;

  CityItem * ci1, *ci2, *ufo;
  Mouse * mouse;
  std::list<CityItem *> items;
  std::list<CityItem *> temp_items;
  std::list<PewPewItem *> shots;
  ci1 = new DimensionGate();
  ci2 = new DimensionGate();
  ufo = new Ufo(ci1, ci2);
  mouse = new Mouse();

  ci1->images = ptang;
  ci2->images = ptang;
  ci2->frame = ci1->frame = 42.0f;
  ci1->start_frame = ci2->start_frame = 42;
  ci1->end_frame = ci2->end_frame = 73;

  ufo->images = ufos;
  ufo->frame = 92;
  ufo->start_frame = (int)ufo->frame;
  ufo->end_frame = 94;

  mouse->start_frame = mouse->end_frame = 0;
  mouse->frame = 0;
  mouse->tx = 10.0f; mouse->ty = 10.0f; mouse->tz = 9.0f;
  mouse->image = mouse_img;

  items.push_back(ci1);
  items.push_back(ci2);
  items.push_back(ufo);
  items.push_back(mouse);

  FpsTimer timer(60);

  int my_sampler_uniform_location = glGetUniformLocation(sp.get_sp(), "tex");
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(my_sampler_uniform_location, GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE0); // We use texture 0 for images
        glBindTexture(GL_TEXTURE_2D, city->getSprite(140)->tex_id);

  while (!quit)
  {
    timer.startOfFrame();


    /* Update animations etc */
    list<CityItem*> to_remove;

  for(std::list<CityItem*>::iterator it = items.begin(); it != items.end(); ++it)
    {
      (*it)->update();
    }

  for(std::list<CityItem*>::iterator it = temp_items.begin(); it != temp_items.end(); ++it)
    {
      (*it)->update();
      if ((*it)->garbage())
        to_remove.push_back(*it);
    }

    for (list<PewPewItem*>::iterator it=shots.begin(); it!=shots.end(); )
    {
      PewPewItem *i=*it;
      i->update();

      if (i->is_hit()) // process hit
      {
        if ((int)i->tz==0)
          cm.setTile((unsigned int)i->tx, (unsigned int)i->ty, (unsigned int)i->tz, Tile(1));
        else
          cm.setTile((unsigned int)i->tx, (unsigned int)i->ty, (unsigned int)i->tz, Tile(0));

        for (int x=-1; x<2; ++x)
          for (int y=-1; y<2; ++y)
            {
              SingleShotItem * shot = new SingleShotItem();

              shot->tx = i->tx+x;
              shot->ty = i->ty+y;
              shot->tz = i->tz;
              shot->images = ptang;
              if (x!=0 || y!=0)
              {
                shot->start_frame = 29;
                shot->frame = 29;
                shot->end_frame=41;
              }
              else
              {
                shot->start_frame = 74;
                shot->frame = 74;
                shot->end_frame=83;
              }
              temp_items.push_back(shot);
            }
      }

      if (i->garbage())
      {
        it = shots.erase(it);
        delete i;
      }
      else
          ++it;
    }

  for(std::list<CityItem*>::iterator it = to_remove.begin(); it != to_remove.end(); ++it)
    {
      CityItem * ci = *it;
      temp_items.remove(ci);
      delete ci;
    }
  to_remove.clear();

    /* Drawing */
    screen->clear();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    sp.use();

    int param = glGetUniformLocation(sp.get_sp(), "mousePos");
    glUniform2f(param, (GLfloat)mouse->sx, (GLfloat)HEIGHT-mouse->sy);

    for (int tz = 0; tz<10; tz++)
    {
      int ftx = 0, ltx=100, fty=0, lty=100;
      const Tile * t = NULL;

      for (int ty=fty; ty<lty; ++ty)
      {
        t = cm.getTileLine(ty, tz);
        int sx, sy;
        for (int tx=ftx; tx<ltx; ++tx)
        {
          Utils::tile_to_screen((float)tx, (float)ty, (float)tz, sx, sy);
          if (sx+TILE_WIDTH > camera.x && sy+TILE_HEIGHT+15> camera.y && sx - camera.x <camera.w && sy - camera.y <camera.h)
          {
            sx -= camera.x;
            sy -= camera.y;
            s.x = sx;
            s.y = sy;
            if (t[tx])
            {
              if (t[tx] < city->count())
                city->getSprite(t[tx])->renderAt(s, sp,(float)tz);
              else
                invalid->getSprite(1)->renderAt(s, sp,(float)tz);
            }
          }
        }
      }

  for(std::list<CityItem*>::iterator it = temp_items.begin(); it != temp_items.end(); ++it)
    {
        int sx, sy;
        if ((int)(*it)->tz == tz)
        {
          Utils::tile_to_screen((*it)->tx, (*it)->ty, (*it)->tz, sx, sy);
          sx -= camera.x;
          sy -= camera.y;
          s.x = sx;
          s.y = sy;
          // Don't clip for now - SDL should do the clipping anyway
          (*it)->renderAt(s, sp, (float)tz);
        }
      }

  for(std::list<CityItem*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        int sx, sy;
        if ((int)(*it)->tz == tz)
        {
          Utils::tile_to_screen((*it)->tx, (*it)->ty, (*it)->tz, sx, sy);
          sx -= camera.x;
          sy -= camera.y;
          s.x = sx;
          s.y = sy;
          // Don't clip for now - SDL should do the clipping anyway
          (*it)->renderAt(s, sp, (float)tz);
        }
      }

  for(std::list<PewPewItem*>::iterator it = shots.begin(); it != shots.end(); ++it)
    {
        int sx, sy;
        if ((int)(*it)->tz == tz)
        {
          Utils::tile_to_screen((*it)->tx, (*it)->ty, (*it)->tz, sx, sy);
          sx -= camera.x;
          sy -= camera.y;
          s.x = sx;
          s.y = sy;
          // Don't clip for now - SDL should do the clipping anyway
          (*it)->renderAt(s, sp,(float)tz);
        }
      }
    }

    sp.unuse();
    int c = cb.buildingCount();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int i=0; i<c; ++i)
    {
        CityBuilding * x = cb.getBuilding(i);
        struct {
            int x; int y;
        } p[4];
        for (int i=0; i<3; ++i)
        {
            //Why they don't fit?
        Utils::tile_to_screen(x->rec.x1, x->rec.y1, i, p[0].x, p[0].y);
        Utils::tile_to_screen(x->rec.x2+1, x->rec.y1, i, p[1].x, p[1].y);
        Utils::tile_to_screen(x->rec.x2+1, x->rec.y2+1, i, p[2].x, p[2].y);
        Utils::tile_to_screen(x->rec.x1, x->rec.y2+1, i, p[3].x, p[3].y);

        glBegin(GL_QUADS);
        glColor3f( 1.0, 1.0, 0.0 ); glVertex3f(p[0].x - camera.x, p[0].y- camera.y, 0);
        glColor3f( 1.0, 1.0, 0.0 ); glVertex3f(p[1].x- camera.x, p[1].y- camera.y, 0);
        glColor3f( 1.0, 1.0, 0.0 ); glVertex3f(p[2].x- camera.x, p[2].y- camera.y, 0);
        glColor3f( 1.0, 1.0, 0.0 ); glVertex3f(p[3].x- camera.x, p[3].y- camera.y, 0);
        glEnd();
        }
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    s.x =0;
    s.y =0;

    sp.use();
		menu->renderAt(s, sp,11);
    mouse->renderAt(s, sp,11);

    screen->flip();

    /* Shot at the city! */
    if (rand() % 10 > 8)
    {
      PewPewItem *pp = new PewPewItem(cm);
      pp->tx=ufo->tx+1;
      pp->ty=ufo->ty+1;
      pp->tz=ufo->tz;
      pp->dx=(float)(rand() % 100);
      pp->dy=(float)(rand() % 100);
      pp->dz=0.0f;
      pp->images=pequip;
      pp->start_frame = 75;
      pp->frame = 75.0f;
      pp->end_frame=75;
      shots.push_back(pp);
    }

    /* EVENT processing */

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        quit = true;
      if (event.type == SDL_KEYDOWN)
        switch (event.key.keysym.sym)
        {
          case SDLK_q:
          case SDLK_ESCAPE: quit = true; break;
        case SDLK_z:
        {
          SingleShotItem * shot;
          for (int i=0; i<4; ++i)
          {
            shot = new SingleShotItem();
            shot->tx = mouse->tx;
            shot->ty = mouse->ty;
            shot->tz = mouse->tz;
            shot->anim_speed = 0.1f;
            shot->images = ptang;
            int q =0;
            switch(i)
            {
              case 0: q=160; break;
            case 1: q=204; shot->tx++; break;
            case 2:q=194; shot->tx++; shot->ty++; break;
            case 3:q=184; shot->ty++; break;
            }
            shot->start_frame =q;
            shot->frame = (float)q;
            shot->end_frame = shot->start_frame + 9;
            temp_items.push_back(shot);
          }
        }
          break;
        case SDLK_f:
        {
          PewPewItem *pp = new PewPewItem(cm);
          pp->tx=ufo->tx;
          pp->ty=ufo->ty;
          pp->tz=ufo->tz;
          pp->dx=(float)(rand() % 100);
          pp->dy=(float)(rand() % 100);
          pp->dz=0.0f;
          pp->images=pequip;
          pp->start_frame = 75;
      pp->frame = 75.0f;
          pp->end_frame=75;
          shots.push_back(pp);
        } break;
        default: break;
        }
      if (event.type == SDL_MOUSEBUTTONUP)
      {
        SingleShotItem * shot = new SingleShotItem();

        shot->tx = mouse->tx;
        shot->ty = mouse->ty;
        shot->tz = mouse->tz;
        shot->anim_speed=1;
        shot->images = ptang;
        switch(event.button.button)
        {
        case SDL_BUTTON_RIGHT:
          shot->start_frame = 8; shot->end_frame = 25;
          break;

        case SDL_BUTTON_MIDDLE:
          shot->start_frame =74; shot->end_frame=83;
          break;

        case SDL_BUTTON_LEFT:
          shot->start_frame =0; shot->end_frame=7;
          break;

        default:
          shot->start_frame =29; shot->end_frame=41;
          break;
          break;
        }
        shot->frame = shot->start_frame - shot->anim_speed;
        temp_items.push_back(shot);
      }
    }

    Uint8 *keystates = SDL_GetKeyState(NULL);
    
    if (keystates[SDLK_LSHIFT])
    {
        AppSettings::SetScrollSpeed(AppSettings::GetScrollSpeed() + 1.0f);
    }
    else
    if(keystates[SDLK_LCTRL])
    {
        AppSettings::SetScrollSpeed(AppSettings::GetScrollSpeed() - 1.0f);
    }

    if(keystates[SDLK_0])
        AppSettings::DefaultScrollSpeed();

    float speed = AppSettings::GetScrollSpeed();

    if (keystates[SDLK_UP])
      camera.y-=speed;
    if (keystates[SDLK_DOWN])
      camera.y+=speed;
    if (keystates[SDLK_LEFT])
      camera.x-=speed;
    if (keystates[SDLK_RIGHT])
      camera.x+=speed;
    if (keystates[SDLK_HOME])
    {
      int cx, cy;
      Utils::tile_to_screen(ufo->tx, ufo->ty, ufo->tz, cx, cy);
      camera.x = cx - AppSettings::GetWindowWidth()/2;
      camera.y = cy - AppSettings::GetWindowHeight()/2;
    }
    if (keystates[SDLK_LALT])
    {
      SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
      SDL_WarpMouse(AppSettings::GetWindowWidth()/2, AppSettings::GetWindowHeight()/2);
      SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    }
    mouse->camera = camera;

    timer.endOfFrame();
  }

  //Quit
  SDL_Quit();
  AppSettings::Free();
  return 0;
}
