/// \file ILoader.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Abstract Interface for loader of many model formats.
/// 
/// File contains abstract class of loader model files of any format
/// which can you load and use. This class are elementary types which 
/// model loader factory can operates.

#ifndef __FRAMEWORK_ILOADER_HPP__
#define __FRAMEWORK_ILOADER_HPP__

#include "Mesh.hpp"

namespace g3d
{
	/// \brief iloader class.
	///
	/// Abstract class for loading files with geometry and actors. Interface will change in future.
	class TFNFRAMEWORK_EXPORT iloader
	{
		public:
			virtual void load_file(const std::string& filename, mesh& outputmesh) = 0;
	};
	
	
	/// \brief ply_loader class.
	///
	/// Class which load a PLY files (geometry and data from this).
	class TFNFRAMEWORK_EXPORT ply_loader : public iloader
	{
		private:
			math::scalar _scale;
			bool _inversed_normals;
			
		public:
			ply_loader(bool normals = false, math::scalar s = math::scalar(1.0)): 
				_scale(s), _inversed_normals(normals)
			{}
		
			virtual void load_file(const std::string& filename, mesh& outputmesh);
	};

} // end namespace g3d

namespace err
{
	/// \brief Loader exception class.
	///
	/// User which develop a new class inherrited from iloader, must use this class to 
	/// throwing errors and indicate unexpected situations. Everybody use this class to catching
	/// exceptions from loaders.
	class TFNFRAMEWORK_EXPORT loader_exception : public framework_exception
	{
		public:
			loader_exception(const std::string& message);
			
			virtual ~loader_exception() throw() {}
			
			const char* what() const throw();
			
		private:
			mutable std::string _cause;
	};

} // end namespace err

#endif