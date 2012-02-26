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
#include "../Framework/PredefSphere.hpp"
#include "../Framework/PredefTorus.hpp"
#include "../Framework/PredefPlane.hpp"
#include "../Framework/PredefBox.hpp"

using namespace g3d;
using namespace utl;
using namespace math;

class cg_window : public window
{
	public:
		predef_sphere msh;
		predef_torus msh2;
		predef_box msh3;
		predef_plane msh4;
		
		std::string fpsstr;				
		uint32_t fr;
		
		float Lightangx;	
		
		vec3 eye;
		color4 col;
		vec3 pos;
		
		renderer::settings full;
		
		vec3 redPlasticEmissive;
		vec3 redPlasticAmbient;
		vec3 redPlasticDiffuse;
		vec3 redPlasticSpecular;
		scalar redPlasticShininess;
	    
		vec3 graySteelEmissive;
		vec3 graySteelAmbient;
		vec3 graySteelDiffuse;
		vec3 graySteelSpecular;
		scalar graySteelShininess;            		
	
		virtual void user_init()
		{	
			tex::initialise();							
			msh.fill(5.0f, vec3::ZERO_VEC3, 64, 64, color4::BLACK4);
			msh2.fill(5.0f, 2.0f, vec3(0.0, -6.0f, 0.0f), 90, color4::GRAY4);
			msh3.fill(5.0f, vec3(0.0, 7.5f, 0.0f), color4::BLACK4);
			msh4.fill(10.0f, vec3(0.0f, -11.0f, 0.0f), ONE, color4::BLACK4);
			
			Lightangx = 0.0f;
		
			fr = 0;
			fpsstr = "Wait for results...";			
			
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
			
			full = sett2;
			
			redPlasticEmissive = vec3(0.0f,  0.0f,  0.0f);
			redPlasticAmbient = vec3(0.0f, 0.0f, 0.0f);
			redPlasticDiffuse =	vec3(0.5f, 0.0f, 0.0f);		
			redPlasticSpecular = vec3(0.7f, 0.6f, 0.6f);
			redPlasticShininess = 32.0f;			
			
			graySteelEmissive = vec3(0.0f,  0.2f, 0.2f);
			graySteelAmbient = vec3(0.0f, 0.0f, 0.0f);
			graySteelDiffuse = vec3(0.9f, 0.9f, 0.9f);
			graySteelSpecular = vec3(0.9f, 0.9f, 0.9f);
			graySteelShininess = 75.0f;     
			
			FONTMANAGER.add("fps", 12, "cour.ttf");
			CGPROGRAMMANAGER.open_program("basicV.cg", "basicV", RWCP_VERTEX);
			CGPROGRAMMANAGER.open_program("basicF.cg", "basicF", RWCP_FRAGMENT);			
			
			pos = vec3(10.0f, 0.0f, 0.0f);
			col = color4(1.0f, 0.0f, 0.0f, 1.0f);
			eye = vec3(0.0f, 5.0f, 30.0f);

			color4 ambient(0.2f, 0.2f, 0.2f, 0.2f);
						
			CGPROGRAMMANAGER["basicV"].set_parameter("lightColor", col);			
			CGPROGRAMMANAGER["basicV"].set_parameter("globalAmbient", ambient);
									
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

			if (key_state('Z')) 
			{
				Lightangx += 0.1f;
				pos.x = 10.0f * sinf(Lightangx);
				pos.z = 10.0f * cosf(Lightangx);
			}
			if (key_state('X')) 
			{
				Lightangx -= 0.1f;			
				pos.x = 10.0f * sinf(Lightangx);
				pos.z = 10.0f * cosf(Lightangx);
			}				

			if (key_state('D'))		pos.y += 0.2f;			
			if (key_state('C'))		pos.y -= 0.2f;			
			
			if (key_state('F'))		RENDERER.change_settings(full, this->window_handle());
			
			if (key_state('Q'))		RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_LINES);
			if (key_state('W'))		RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_FILL);
					  			
			FONTMANAGER["fps"].print(-400.0f, 330.0f, fpsstr, color4::WHITE4);						  			
								
			CGPROGRAMMANAGER["basicV"].enable_profile();			
			CGPROGRAMMANAGER["basicV"].bind_program();										

			CGPROGRAMMANAGER["basicF"].enable_profile();
			CGPROGRAMMANAGER["basicF"].bind_program();						
												
			CGPROGRAMMANAGER["basicV"].set_parameter("eyePosition", eye);												
			CGPROGRAMMANAGER["basicV"].set_parameter("lightPosition", vec4(pos.x, pos.y, pos.z, 1.0f));
			
			RENDERER.disable(RWS_TEXTURE_2D);
			glPointSize(4.0f);
			glBegin(GL_POINTS);
				glVertex3f(pos.x, pos.y, pos.z);								
			glEnd();
			RENDERER.enable(RWS_TEXTURE_2D);
			
			glPushMatrix();
				glTranslatef(0.0, 0.0, -10.0f);				
				CGPROGRAMMANAGER["basicV"].set_renderer_matrix("modelViewProj");	
													
				CGPROGRAMMANAGER["basicV"].set_parameter("Ke", redPlasticEmissive);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ks", redPlasticSpecular);
				CGPROGRAMMANAGER["basicV"].set_parameter("Kd", redPlasticDiffuse);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ka", redPlasticAmbient);
				CGPROGRAMMANAGER["basicV"].set_parameter("shininess", redPlasticShininess);				
				msh.draw(CGPROGRAMMANAGER["basicV"]);		
								
				CGPROGRAMMANAGER["basicV"].set_parameter("Ke", graySteelEmissive);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ks", graySteelSpecular);
				CGPROGRAMMANAGER["basicV"].set_parameter("Kd", graySteelDiffuse);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ka", graySteelAmbient);
				CGPROGRAMMANAGER["basicV"].set_parameter("shininess", graySteelShininess);								
				msh2.draw(CGPROGRAMMANAGER["basicV"]);		
				
				CGPROGRAMMANAGER["basicV"].set_parameter("Ke", graySteelEmissive);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ks", graySteelSpecular);
				CGPROGRAMMANAGER["basicV"].set_parameter("Kd", graySteelDiffuse);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ka", graySteelAmbient);
				CGPROGRAMMANAGER["basicV"].set_parameter("shininess", graySteelShininess);								
				msh3.draw(CGPROGRAMMANAGER["basicV"]);				
				
				CGPROGRAMMANAGER["basicV"].set_parameter("Ke", redPlasticEmissive);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ks", redPlasticSpecular);
				CGPROGRAMMANAGER["basicV"].set_parameter("Kd", redPlasticDiffuse);
				CGPROGRAMMANAGER["basicV"].set_parameter("Ka", redPlasticAmbient);
				CGPROGRAMMANAGER["basicV"].set_parameter("shininess", redPlasticShininess);		
				msh4.draw(CGPROGRAMMANAGER["basicV"]);				
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