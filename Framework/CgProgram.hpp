/// \file CgProgram.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Interface of Cg program.
/// 
/// File contains interface of cg program, operations and some usability
/// tricks for better use.

#ifndef __FRAMEWORK_CGPROGRAM_HPP__
#define __FRAMEWORK_CGPROGRAM_HPP__

#include "../Common/MathVec.hpp"
#include "../Common/MathMat.hpp"
#include "../Common/MathColor.hpp"

namespace g3d
{
	/// \brief cg_program class.
	///
	/// Contains cg program, and operates on it. Compiling, loading, 
	/// getting arguments - that's all things which this class doing.
	/// It can be copied.
	class TFNFRAMEWORK_EXPORT cg_program
	{
		public:
			cg_program();
			~cg_program();
			
			void load_from_file(const std::string& filename, CGcontext context, RW_CG_PROGRAM rwcp,
								RW_CG_PROGRAM_TYPE rwcpt, const std::string& entry = "main", const std::string& args = "");
			void destroy();
			
			void compile_program() const;
			bool is_program_compiled() const;
			
			std::string get_listing(CGcontext context) const;
			
			void set_parameter(const std::string& paramname, const math::vec4& values);
			void set_parameter(const std::string& paramname, const math::vec3& values);
			void set_parameter(const std::string& paramname, const math::color4& values);
			void set_parameter(const std::string& paramname, const math::color3& values);						
			void set_parameter(const std::string& paramname, const math::scalar& value);	
			void set_parameter(const std::string& paramname, const math::mat4& values);
			
			void set_texture(const std::string& paramname, utl::uint32_t id);
			
			void set_renderer_matrix(const std::string& paramname);
			void set_renderer_transposed_matrix(const std::string& paramname);
			void set_world_matrix(const std::string& paramname);
			void set_world_transposed_matrix(const std::string& paramname);
			void set_world_inverted_transposed_matrix(const std::string& paramname);
			void set_world_inverted_matrix(const std::string& paramname);
			
			void set_profile(CGprofile prof)				{	_profile = prof;	}
			CGprofile get_profile() const					{	return _profile;	}
						
			void enable_profile() const;
			void disable_profile() const;
			
			void bind_program() const;
			
			CGprogram program() const						{ return _program;		}
			
		private:
			CGprogram _program;
			CGprofile _profile;
			
			RW_CG_PROGRAM _type;
			RW_CG_PROGRAM_TYPE _rwcpt;
			
			std::string _filename;
			
			// Noncopyable idiom.
			cg_program(const cg_program&);
			cg_program& operator=(const cg_program&);
	};

} // end namespace g3d

#endif