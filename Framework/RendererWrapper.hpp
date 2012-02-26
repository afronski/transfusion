/// \file RendererWrapper.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 02.03.2009
///
/// \brief Renderer constants and enumerations wrapper.
/// 
/// File contains enumerations, and constants for Renderer class.

#ifndef __RENDERERWRAPPER_FRAMEWORK_HPP__
#define __RENDERERWRAPPER_FRAMEWORK_HPP__

/// \brief Main namespace of framework
///
/// Contains every class, structure, function, constant
/// involved with 3D graphics in the engine.
namespace g3d
{
	/// \brief Texture wrapping mode.
	/// 
	/// Informs renderer how to wrap texture on the edges.
	enum RW_WRAPING_MODE
	{
		RWWM_CLAMP = GL_CLAMP,							///< Clamp.
		RWWM_REPEAT = GL_REPEAT,						///< Repeat.
		RWWM_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER_ARB,	///< Clamp to border.
		RWWM_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE_EXT,		///< Clamp to edge.
		RWWM_MIRRORED_REPEAT = GL_MIRRORED_REPEAT_ARB,	///< Mirrored repeat.
		RWWM_MIRROR_CLAMP = GL_MIRROR_CLAMP_EXT			///< Mirror clamp.
	};
	
	/// \brief Texture minification filtering mode.
	///
	/// Informs renderer how filter texture if it is zoomed in.
	enum RW_MINFILTER_MODE
	{
		RWMINFT_NEAREST = GL_NEAREST,									///< Nearest.
		RWMINFT_LINEAR = GL_LINEAR,										///< Linear.
		RWMINFT_MIPPMAPED_NEAREST_NEAREST = GL_NEAREST_MIPMAP_NEAREST,	///< Nearest-Mipmap-Nearest.
		RWMINFT_MIPPMAPED_LINEAR_NEAREST = GL_LINEAR_MIPMAP_NEAREST,	///< Linear-Mipmap-Nearest.
		RWMINFT_MIPPMAPED_LINEAR_LINEAR = GL_LINEAR_MIPMAP_LINEAR		///< Linear-Mipmap-Linear.
	};
	
	/// \brief Texture magnification filtering mode.
	/// 
	/// Informs renderer how filter texture if it is zoomed out.
	enum RW_MAGFILTER_MODE
	{
		RWMAGFT_NEAREST = GL_NEAREST,			///< Nearest.
		RWMAGFT_LINEAR = GL_LINEAR,				///< Linear.
	};
	
	/// \brief Output Texture format.
	/// 
	/// Specified type of output pixels. 
	enum RW_TEXTURE_OUTPUT_FORMAT
	{
		RWTOF_CHOOSE = -1,								///< Choose ouput texture format.
		RWTOF_ALPHA = GL_ALPHA,							///< Alpha.
		RWTOF_LUMINANCE = GL_LUMINANCE,					///< Luminance.
		RWTOF_LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,		///< Luminance-Alpha.
		RWTOF_RGB = GL_RGB,								///< RGB.
		RWTOF_RGBA = GL_RGBA,							///< RGBA.
		RWTOF_ABGR = GL_ABGR_EXT						///< ABGR.
	};
	
	/// \brief Internal Texture format.
	/// 
	/// Specified internal type texture in memory.
	enum RW_TEXTURE_INTERNAL_FORMAT
	{
		RWTIF_CHOOSE = -1,								///< Choose internal texture format.
		RWTIF_ALPHA = GL_ALPHA,							///< Alpha.
		RWTIF_LUMINANCE = GL_LUMINANCE,					///< Luminance.
		RWTIF_LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,		///< Luminance-Alpha.
		RWTIF_RGB = GL_RGB,								///< RGB.
		RWTIF_RGBA = GL_RGBA,							///< RGBA.
	};
		
	/// \brief Alpha Function.
	/// 
	/// Specified function how compare objects with alpha.
	enum RW_ALPHA_FUNCTION
	{
		RWAF_NEVER = GL_NEVER,							///< Never.
		RWAF_ALWAYS = GL_ALWAYS,						///< Always.
		RWAF_LESS = GL_LESS,							///< Less.
		RWAF_GREATER = GL_GREATER,						///< Greater.
		RWAF_EQUAL = GL_EQUAL,							///< Equal.
		RWAF_LEQUAL = GL_LEQUAL,						///< Lequal.
		RWAF_NOTEQUAL = GL_NOTEQUAL,					///< Notequal.
		RWAF_GEQUAL = GL_GEQUAL							///< Gequal.
	};
		
	/// \brief Polygon Draw Mode.
	///
	/// Specified polygon draw mode.
	enum RW_POLYGON_DRAW_MODE
	{
		RWPDM_POINTS = GL_POINT,		///< Draw points.
		RWPDM_LINES = GL_LINE,			///< Draw line.
		RWPDM_FILL = GL_FILL,			///< Draw filled polygons.
	};		
		
	/// \brief Polygon side.
	///
	/// Specified polygon side, for polygon draw mode.
	enum RW_POLYGON_SIDE
	{
		RWPS_FRONT = GL_FRONT,						///< Front side.
		RWPS_BACK = GL_BACK,						///< Back side.
		RWPS_FRONT_AND_BACK = GL_FRONT_AND_BACK		///< Both sides.
	};
		
	/// \brief Texture Targets.
	///
	/// Specifies texture targets.
	enum RW_TEXTURE_TARGET
	{
		RWTT_TEXTURE_1D = GL_TEXTURE_1D,		///< 1D Texture.
		RWTT_TEXTURE_2D = GL_TEXTURE_2D			///< 2D Texture.
	};
	
	/// \brief Matrix Mode.
	///
	/// Specifies on which matrix renderer operates.
	enum RW_MATRIX_MODE
	{
		RWMM_PROJECTION_MATRIX = GL_PROJECTION,	///< Projection matrix.
		RWMM_MODELVIEW_MATRIX = GL_MODELVIEW,	///< Modelview matrix.
		RWMM_TEXTURE_MATRIX = GL_TEXTURE		///< Texture matrix.
	};
		
	/// \brief Smooth mode.
	///
	/// Specifies shading mode.
	enum RW_SMOOTH_MODE
	{
		RWSM_FLAT = GL_FLAT,		///< Flat shading.
		RWSM_SMOOTH = GL_SMOOTH		///< Smooth shading.
	};
		
	/// \brief Machine States.
	///
	///	Specifies enabled/disabled renderer states.
	enum RW_STATES
	{
		RWS_BLENDING = GL_BLEND,			///< Blending.
		RWS_DEPTH_TEST = GL_DEPTH_TEST,		///< Depth test.
		RWS_ALPHA_TEST = GL_ALPHA_TEST,		///< Alpha test.
		RWS_LIGHTING = GL_LIGHTING,			///< Lighting.
		RWS_TEXTURE_2D = GL_TEXTURE_2D		///< 2D Texturing.
	};
			
	/// \brief Hardware Array usage. 
	///
	/// Specifies how can often data will be reloaded.
	///	Only three values are specified. Most description is below.
	enum RW_HW_ARRAY_USAGE
	{
		RWHAU_STATIC_ARRAY = 0,		///< Program load only once data.
		RWHAU_DYNAMIC_ARRAY,		///< Program will reload from time to time data, driver shuold allocate AGP memory.
		RWHAU_STREAM_ARRAY			///< Program load only once data, array can be discarded - data are valid only for a one frame.
	};
		
	/// \brief Hardware Array Element Type.
	///
	/// Specifies element type of array element, and size.
	enum RW_HW_ARRAY_ELEMENT_TYPE
	{
		RWHAET_FLOAT = sizeof(float),			///< Float.
		RWHAET_DOUBLE = sizeof(double),			///< Double.
		RWHAET_UBYTE = sizeof(unsigned char)	///< Unsigned char.
	};
	
	/// \brief Hardware Array Element Size.
	///
	/// Specifies size of element hardware arrays.
	enum RW_HW_ARRAY_ELEMENT_SIZE
	{
		RWHAES_1 = 1,			///< One element.
		RWHAES_2 = 2,			///< Two elements.				
		RWHAES_3 = 3,			///< Three elements.
		RWHAES_4 = 4,			///< Four elements.
	};		
	
	/// \brief Hardware Array Type.
	///
	/// Type of hardware vertex array.
	enum RW_HW_ARRAY_TYPE
	{
		RWHAT_POSITION = GL_VERTEX_ARRAY,			///< Position array.
		RWHAT_NORMAL = GL_NORMAL_ARRAY,				///< Normal array.
		RWHAT_COLOR = GL_COLOR_ARRAY,				///< Color array.
		RWHAT_TEXCOORD = GL_TEXTURE_COORD_ARRAY		///< Texture coords array.
	};	
		
	/// \brief Hardware Index Array Element Type
	///
	/// Specifies array element type, and size.
	enum RW_HW_INDEX_ARRAY_ELEMENT_TYPE
	{
		RWHIAET_UINT = sizeof(unsigned int),		///< Unsigned int.
		RWHIAET_USHORT = sizeof(unsigned short)		///< Unsigned short.
	};		
		
	/// \brief Hardware Internal Element Type.
	///
	/// Internal format of element in hardware vertex/index array.
	enum RW_HW_INTERNAL_ELEMENT_TYPE 
	{
		RWHIET_FLOAT = GL_FLOAT,			///< Float.
		RWHIET_DOUBLE = GL_DOUBLE,			///< Double.
		RWHIET_UBYTE = GL_UNSIGNED_BYTE,	///< Unsigned char.
		
		RWHIET_UINT = GL_UNSIGNED_INT,		///< Unsigned int.
		RWHIET_USHORT = GL_UNSIGNED_SHORT	///< Unsigned short.
	};

	/// \brief Cg program type.
	///
	/// Internal type of cg program, loaded from outside.
	enum RW_CG_PROGRAM_TYPE
	{
		RWCPT_SOURCE = CG_SOURCE,
		RWCPT_OBJECT = CG_OBJECT
	};
	
	/// \brief Cg program.
	///
	/// Type of cg program - vertex, fragment or geometry shader.
	enum RW_CG_PROGRAM
	{
		RWCP_FRAGMENT = CG_GL_FRAGMENT,
		RWCP_VERTEX = CG_GL_VERTEX,
		RWCP_GEOMETRY = CG_GL_GEOMETRY
	};	

} // end namespace 

#endif