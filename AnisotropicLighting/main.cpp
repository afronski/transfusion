#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "../Common/pchCommon.hpp"
#include "../Framework/pchFramework.hpp"

#include "../Common/StringOperations.hpp"
#include "../Common/Timer.hpp"

#include "../Framework/Window.hpp"
#include "../Framework/Renderer.hpp"
#include "../Framework/HardwareArrayManager.hpp"
#include "../Framework/CgProgramManager.hpp"
#include "../Framework/CgContext.hpp"
#include "../Framework/FontManager.hpp"
#include "../Framework/Texture.hpp"
#include "../Framework/PredefTorus.hpp"

using namespace g3d;
using namespace utl;
using namespace math;

class cg_window : public window
{
	public:
		predef_torus msh;
		
		tex2D aniso;
		
		std::string fpsstr;				
		uint32_t fr;

		float Lightangx;	
		float ang;
		
		vec3 eye;
		vec3 pos;
		vec4 LightDir;		
	
		virtual void user_init()
		{	
			tex::initialise();							
			msh.fill(5.0f, 2.5f, vec3::ZERO_VEC3, 90, color4::WHITE4);
			
			Lightangx = ang = 0.0f;
		
			fr = 0;
			fpsstr = "Wait for results...";
									
			aniso.load("aniso.tga");
			texture_settings sett = { RWWM_MIRRORED_REPEAT, RWWM_CLAMP, RWWM_CLAMP,
									  RWMAGFT_LINEAR, RWMINFT_LINEAR, RWTIF_RGBA, RWTOF_RGBA };
			aniso.generate(sett);	
			
			FONTMANAGER.add("fps", 12, "cour.ttf");			
			CGPROGRAMMANAGER.open_program("anisoV.cg", "basicV", RWCP_VERTEX);		
			CGPROGRAMMANAGER.open_program("anisoF.cg", "basicF", RWCP_FRAGMENT);					
			
			pos = vec3(8.0f, 0.0f, 0.0f);
			
			LightDir.x = pos.x;
			LightDir.y = pos.y;
			LightDir.z = pos.z;
			LightDir.w = 1.0f;
			LightDir.normalize();
			
			eye = vec3(0.0f, 5.0f, 30.0f);
			
			RENDERER.disable(RWS_LIGHTING);									
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
			
			gluLookAt(eye.x, eye.y, eye.z, 
					  0.0f, 0.0f, 0.0f,
					  0.0f, 1.0f, 0.0f);

			if (key_state('F')) ang += 0.2f;			
			if (key_state('V'))	ang -= 0.2f;

			if (key_state('Z')) 
			{
				Lightangx += 0.1f;
				pos.x = 8.0f * sinf(Lightangx);
				pos.z = 8.0f * cosf(Lightangx);
				
				LightDir.x = pos.x;
				LightDir.y = pos.y;
				LightDir.z = pos.z;
				LightDir.w = 1.0f;
				LightDir.normalize();
			}
			if (key_state('X')) 
			{
				Lightangx -= 0.1f;			
				pos.x = 8.0f * sinf(Lightangx);
				pos.z = 8.0f * cosf(Lightangx);
				LightDir.x = pos.x;
				LightDir.y = pos.y;
				LightDir.z = pos.z;
				LightDir.w = 1.0f;
				LightDir.normalize();
			}				

			if (key_state('D'))		
			{
				pos.y += 0.2f;			
				LightDir.x = pos.x;
				LightDir.y = pos.y;
				LightDir.z = pos.z;
				LightDir.w = 1.0f;
				LightDir.normalize();
			}				
			if (key_state('C'))		
			{
				pos.y -= 0.2f;	
				LightDir.x = pos.x;
				LightDir.y = pos.y;
				LightDir.z = pos.z;
				LightDir.w = 1.0f;
				LightDir.normalize();
			}
			
			if (key_state('Q'))		RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_LINES);
			if (key_state('W'))		RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_FILL);
					  			
			FONTMANAGER["fps"].print(-400.0f, 330.0f, fpsstr, color4::WHITE4);						  			
								
			CGPROGRAMMANAGER["basicV"].enable_profile();			
			CGPROGRAMMANAGER["basicV"].bind_program();				
																												
			CGPROGRAMMANAGER["basicF"].enable_profile();			
			CGPROGRAMMANAGER["basicF"].bind_program();							
												
			CGPROGRAMMANAGER["basicV"].set_parameter("EyePos", eye);												
			CGPROGRAMMANAGER["basicV"].set_parameter("LightVec", LightDir);	
								
			CGPROGRAMMANAGER["basicF"].set_texture("tex0", aniso.ID());		
						
			RENDERER.disable(RWS_TEXTURE_2D);
			glPointSize(4.0f);
			glBegin(GL_POINTS);
				glVertex3f(pos.x, pos.y, pos.z);								
			glEnd();
			RENDERER.enable(RWS_TEXTURE_2D);

			RENDERER.bind_texture(RWTT_TEXTURE_2D, aniso.ID());
			glPushMatrix();	
				glRotatef(ang, 1.0f, 0.0f, 0.0f);
				
				CGPROGRAMMANAGER["basicV"].set_renderer_matrix("WorldViewProj");	
				CGPROGRAMMANAGER["basicV"].set_world_inverted_transposed_matrix("WorldIT");
				CGPROGRAMMANAGER["basicV"].set_world_transposed_matrix("World");
																		
				msh.draw(CGPROGRAMMANAGER["basicV"]);				
			glPopMatrix();
					
			CGPROGRAMMANAGER["basicV"].disable_profile();			
			CGPROGRAMMANAGER["basicF"].disable_profile();			
		}
};

int WINAPI WinMain(HINSTANCE HInst, HINSTANCE HPrev, LPSTR lpszCmdLine, int cmdShow)
{	
	int32_t RET = -1;	
	
	try
	{	
		std::auto_ptr<hires_timer> mTimer(new hires_timer());
		mTimer->create();
		
		std::auto_ptr<renderer> mRenderer(new renderer());		
		std::auto_ptr<hardware_array_manager> mArrayManager(new hardware_array_manager());
		std::auto_ptr<font_manager> mFontSys(new font_manager());
		mFontSys->initialise();	

		std::auto_ptr<cg_program_manager> mProgramManager(new cg_program_manager());	
		mProgramManager->initialise();
		
		std::auto_ptr<window> mWindow(new cg_window());										
		mWindow->create(HInst);
		
		RET = mWindow->run();		
						
		mFontSys->deinitialise();
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