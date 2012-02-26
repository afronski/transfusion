#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "../Common/pchCommon.hpp"
#include "../Framework/pchFramework.hpp"

#include "../Framework/Window.hpp"
#include "../Framework/Texture.hpp"

#include "../Common/StringOperations.hpp"
#include "../Common/Logger.hpp"
#include "../Common/Timer.hpp"
#include "../Common/Profiler.hpp"

#include "../Framework/FontManager.hpp"

#include "../Framework/ILoader.hpp"

#include "../Framework/PredefBox.hpp"
#include "../Framework/PredefPlane.hpp"	
#include "../Framework/PredefSphere.hpp"
#include "../Framework/PredefTorus.hpp"

#include "../Framework/HardwareArrayManager.hpp"
#include "../Framework/Renderer.hpp"
#include "../Framework/Mesh.hpp"

using namespace g3d;
using namespace utl;
using namespace math;

class my_window : public window
{
	public:
		mesh msh;		
		tex2D tex;
		
		std::string fpsstr;
		
		double angx, angy;		
		uint32_t fr;
		
		renderer::settings full;
		renderer::settings screen;
		
		predef_box b;
		predef_plane p;
		predef_sphere s;
		predef_torus t;
	
		virtual void user_init()
		{			
			tex::initialise();				
			
			PROFILER.start("PLY loading");				
				std::auto_ptr<g3d::iloader> plyloader(new ply_loader(false, 5.0f));
				plyloader->load_file("monkey.ply", msh);			
			PROFILER.stop();
						
			fr = 0;
			fpsstr = "Wait for results...";
			angx = angy = 0.0;														
			
			tex.load("Test.tga");
			tex.generate_mipmaps();
						
			b.fill(4.0f, vec3(10.0f, -5.0f, 0.0f));			
			p.fill(10.0f, vec3(0.0f, -0.5f, 0.0f), 1.0f);
			s.fill(4.0f, vec3(-10.0f, 5.0f, 0.0f), 32, 32);
			t.fill(3.0f, 2.7f, vec3(10.0f, 5.0f, 0.0f), 30);				
			
			renderer::settings sett =  
			{
				1024, 768, 32, 60,
				24, 0,
				false,
				true,
				color4::BLACK4,
				renderer::settings::EX_STYLE_WINDOWED, 
				renderer::settings::STYLE_WINDOWED
			};				
			
			renderer::settings sett2 =  
			{
				1024, 768, 32, 60,
				24, 0,
				true,
				true,
				color4::BLACK4,
				renderer::settings::EX_STYLE_FULLSCREEN, 
				renderer::settings::STYLE_FULLSCREEN			
			};			
						
			screen = sett;										
			full = sett2;																																
			
			RENDERER.change_settings(screen, window_handle());

			FONTMANAGER.add("comic", 70, "comic.ttf");
			FONTMANAGER.add("fps", 16, "cour.ttf");
			FONTMANAGER.add("ihcs", 36, "ihcs.ttf");
			FONTMANAGER.add("orange", 26, "orange.ttf");
			FONTMANAGER.add("whopass", 36, "whopass.ttf");
			FONTMANAGER.add("potty", 76, "potty.ttf");
			FONTMANAGER.add("bigbb", 36, "bigbb.ttf");
			FONTMANAGER.add("charbb", 36, "charbb.ttf");
									
			scalar pos[4]  = { 0.0f, 5.0f, -5.0f, 1.0f };
			scalar amb[4]  = { 0.8f, 0.8f, 0.8f, 1.0f  };
			scalar diff[4] = { 0.4f, 0.4f, 1.0f, 1.0f  };
			
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
			glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			
			RENDERER.enable(RWS_LIGHTING);
			glEnable(GL_LIGHT0);	

			RENDERER.enable(RWS_TEXTURE_2D);
			RENDERER.enable(RWS_DEPTH_TEST);
		}
		
		virtual void user_destroy()
		{
			tex::deinitialise();
		}
		
		virtual void update(scalar dt)
		{
			++fr;
			if (fr == 60)
			{
				fast_format_string(fpsstr, "FPS: %.2f", TIMER.fps(60));
				fr = 0;
			}
		}
		
		virtual void draw()
		{									
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RENDERER.identity_matrix();													
			
			gluLookAt(0.0f, 5.0f, 30.0f, 
					  0.0f, 0.0f, 0.0f,
					  0.0f, 1.0f, 0.0f);

			if (key_state('F')) RENDERER.change_settings(full, window_handle());
			if (key_state('C')) RENDERER.change_settings(screen, window_handle());

			if (key_state(VK_LEFT)) angy -= 0.8;
			if (key_state(VK_RIGHT)) angy += 0.8;

			if (key_state(VK_UP)) angx -= 0.8;
			if (key_state(VK_DOWN)) angx += 0.8;						
			
			if (key_state('P')) RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_LINES);
			if (key_state('O')) RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_POINTS);
			if (key_state('I')) RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_FILL);			
								
			FONTMANAGER["fps"].print(-300.0f, 300.0f, fpsstr, color4::WHITE4);			
			FONTMANAGER["comic"].print(-30.0f, -30.0f, "aSd", color4::GREEN4);
			FONTMANAGER["orange"].print(-500.0f, 40.0f, "Magic :)!", color4::RED4);
			FONTMANAGER["ihcs"].print(-330.0f, -100.0f, "I hate comic sans! ;P", color4::GRAY4);

			FONTMANAGER["bigbb"].print(-300.0f, 120.0f, "transfusion", color4::WHITE4);			
			FONTMANAGER["charbb"].print(-300.0f, 100.0f, "transfusion", color4::WHITE4);			
			FONTMANAGER["potty"].print(-180.0f, 25.0f, "AGCDINKOPUYz", color4(1.0f, 1.0f, 0.0f, 1.0f));
			FONTMANAGER["whopass"].print(-500.0f, -60.0f, "transfusion", color4::GREEN4);			
		
			RENDERER.bind_texture(RWTT_TEXTURE_2D, tex.ID());							
			
			glPushMatrix();
				glRotated(angy, 0.0, 1.0, 0.0);					
				p.draw();				
			glPopMatrix();

			RENDERER.disable(RWS_TEXTURE_2D);
			glPushMatrix();
				glTranslated(-10.0, -5.0, 0.0);
				glRotated(angy, 0.0, 1.0, 0.0);					
				msh.draw();
			glPopMatrix();
			RENDERER.enable(RWS_TEXTURE_2D);
			
			glPushMatrix();	
				glRotated(angy, 0.0, 1.0, 0.0);					
				s.draw();
			glPopMatrix();
			
			glPushMatrix();	
				glRotated(angy, 0.0, 1.0, 0.0);							
				b.draw();
			glPopMatrix();
			
			glPushMatrix();					
				glRotated(angx, 1.0, 0.0, 0.0);				
				t.draw();
			glPopMatrix();
		}
};

int WINAPI WinMain(HINSTANCE HInst, HINSTANCE HPrev, LPSTR lpszCmdLine, int cmdShow)
{	
	int32_t RET = -1;
	
	try
	{							
		std::auto_ptr<log_manager> mLogManager(new log_manager);
		LOG.link("out", new textfile_output("out.txt"));				
		LOG["out"] << prio(PT_INFO) << "Inicjalizacja okna." << eol;
				
		std::auto_ptr<hires_timer> mTimer(new hires_timer());			
		mTimer->create();
	
		std::auto_ptr<renderer> mRenderer(new renderer());	
		std::auto_ptr<hardware_array_manager> mArrayManager(new hardware_array_manager());
		std::auto_ptr<font_manager> mFontSys(new font_manager());
		mFontSys->initialise();
		
		std::auto_ptr<profile_manager> mProfiler(new profile_manager());
		std::auto_ptr<window> mWindow(new my_window());		
		mWindow->create(HInst);	
		
		RET = mWindow->run();		
						
		mFontSys->deinitialise();
		LOG["out"] << prio(PT_INFO) << "Koniec dzia³ania aplikacji." << eol;		
		PROFILER.save_results();
	}	
	catch(const std::exception& e)
	{
		MessageBox(0, e.what(), "Error", MB_ICONERROR | MB_OK);		
	}
	catch(...)
	{
		MessageBox(0, "Undefined exception", "Error", MB_ICONERROR | MB_OK);		
	}
	
	_CrtDumpMemoryLeaks();
	
	return RET;
} 