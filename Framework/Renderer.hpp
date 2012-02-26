#ifndef __FRAMEWORK_RENDERERMACHINE_HPP__
#define __FRAMEWORK_RENDERERMACHINE_HPP__

#include "../Common/MathScalar.hpp"
#include "../Common/MathColor.hpp"

#include "../Common/Exception.hpp"
#include "../Common/Singleton.hpp"

#define RENDERER (g3d::renderer::get())

namespace g3d
{			
	//--------------------------------------------------------------------------------------------
	// Renderer.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT renderer : public utl::singleton<renderer>
	{
		public:
			// Inner structure, to changing settings.
			struct settings
			{
				static const utl::uint32_t STYLE_FULLSCREEN = WS_POPUP;
				static const utl::uint32_t EX_STYLE_FULLSCREEN = WS_EX_TOPMOST;
				
				static const utl::uint32_t STYLE_WINDOWED =  WS_OVERLAPPEDWINDOW;
				static const utl::uint32_t EX_STYLE_WINDOWED = 0;
				
				utl::uint32_t Width;
				utl::uint32_t Height;
				utl::uint32_t BPP;
				utl::uint32_t RefHz;
									
				utl::uint8_t DepthBits;
				utl::uint8_t StencilBits;
				
				bool Fullscreen;
				bool VSync;
				
				math::color4 ClearColor;
				
				utl::uint32_t ExWindowStyle;
				utl::uint32_t WindowStyle;
			};
			
			renderer();
			~renderer();
					
			void init();
			void change_settings(const settings& sett, HWND hwnd);		
			void adjust_window();	

			// Accessors.
			utl::uint32_t get_width() const	{ return _actualsettings.Width;		}
			utl::uint32_t get_height() const { return _actualsettings.Height;	}
			utl::uint32_t get_bpp() const	{ return _actualsettings.BPP;		}
			utl::uint32_t get_refhz() const	{ return _actualsettings.RefHz;		}
			
			utl::uint8_t get_depth_bits() const		{ return _actualsettings.DepthBits;		}	
			utl::uint8_t get_stencil_bits() const	{ return _actualsettings.StencilBits;	}				
				
			bool get_fullscreen() const		{ return _actualsettings.Fullscreen;	}
			bool get_vsync() const			{ return _actualsettings.VSync;			}
				
			math::color4 get_clear_color() const	{ return _actualsettings.ClearColor;		}
				
			utl::uint32_t get_ex_window_style() const	{ return _actualsettings.ExWindowStyle;		}
			utl::uint32_t get_window_style() const		{ return _actualsettings.WindowStyle;		}
			
			std::string version() const;
			bool check_extension(const std::string& ext) const;			
			
			// Ustawienia renderera.
			void set_vsync() const;
			void set_clear_color() const;
			
			void set_ortho() const;
			void return_from_ortho() const;
			
			void set_perspective(math::scalar fov, math::scalar aspect, math::scalar nearf, math::scalar farf) const;
			
			void polygon_draw_mode(RW_POLYGON_SIDE eglps, RW_POLYGON_DRAW_MODE eglpdm) const;
			void alpha_function(RW_ALPHA_FUNCTION eglaf, math::scalar val) const;
			
			void enable(RW_STATES egls) const;
			void disable(RW_STATES egls) const;
			
			void push_all_states() const;
			void pop_all_states() const;
			
			void bind_texture(RW_TEXTURE_TARGET egltt, utl::uint32_t id) const;
			
			void set_viewport(utl::uint32_t x1, utl::uint32_t y1, utl::uint32_t x2, utl::uint32_t y2) const;
			
			void matrix_mode(RW_MATRIX_MODE eglmm) const;
			void identity_matrix() const;
			
			void shade_model(RW_SMOOTH_MODE eglsm) const;
			
		private:
			settings _actualsettings;	
			
			// Noncopyable idiom.
			renderer(const renderer&);
			renderer& operator=(const renderer&);
	};
	
} // end namespace g3d

namespace err
{
	class TFNFRAMEWORK_EXPORT renderer_exception : public framework_exception
	{
		public:
			renderer_exception(const std::string& message);
			
			virtual ~renderer_exception() throw() {}
			
			const char* what() const throw();
			
		private:
			mutable std::string _cause;
	};

} // end namespace err

#endif