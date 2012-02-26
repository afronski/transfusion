#include "pchFramework.hpp"

#include "../Common/Tokenizer.hpp"

#include "../Common/MathVec.hpp"

#include "../Common/Exception.hpp"
#include "ILoader.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Exceptions.
//--------------------------------------------------------------------------------------------
err::loader_exception::loader_exception(const std::string &message):
	err::framework_exception(message)
{}

const char* err::loader_exception::what() const
{
	if (_cause.empty()) // Leniwa inicjalizacja ³añcucha z b³êdem.
	{
		try
		{
			_cause = this->framework_exception::what();			
		}
		catch(...) { return framework_exception::what(); }
	}
	return _cause.c_str();
}

//--------------------------------------------------------------------------------------------
// Implementation - ply_loader.
//--------------------------------------------------------------------------------------------
void ply_loader::load_file(const std::string& filename, mesh& outputmesh)
{
	std::ifstream in;
	in.open(filename.c_str());
	if (!in.good())
		throw err::loader_exception(std::string("Cannot open PLY file: ") + filename);		
		
	std::string line;	
	std::getline(in, line);	
	if (line.find("ply") == std::string::npos) 
		throw err::loader_exception(std::string("This is not a PLY file: ") + filename);
	
	utl::uint32_t vmax = 0;
	utl::uint32_t faces = 0;
		
	bool nocalcnormal = false;
	
	utl::tokenizer<' '> T;
	
	while(line.find("end_header") == std::string::npos)
	{
		std::getline(in, line);
		if (line.find("element vertex") != std::string::npos)
		{
			T.tokenize(line);
			vmax = atoi(T[2].c_str());
		}
		if (line.find("element face") != std::string::npos)
		{
			T.tokenize(line);
			faces = atoi(T[2].c_str());
		}	
		if (line.find("property math::scalar nx") != std::string::npos) nocalcnormal = true;
	}
	
	std::vector<math::scalar> vertices;
	std::vector<math::scalar> normals;
	std::vector<utl::uint32_t> indices;	
	
	// Wierzcho³ki.
	vertices.resize(vmax * 3);
	normals.resize(vmax * 3);
	for(utl::uint32_t i = 0; i < (vmax * 3); i += 3)
	{
		std::getline(in, line);
		T.tokenize(line);
		
		vertices[i    ] = static_cast<math::scalar>(atof(T[0].c_str()));
		vertices[i + 1] = static_cast<math::scalar>(atof(T[1].c_str()));
		vertices[i + 2] = static_cast<math::scalar>(atof(T[2].c_str()));		

		if (nocalcnormal)
		{
			normals[i    ] = static_cast<math::scalar>(atof(T[3].c_str()));
			normals[i + 1] = static_cast<math::scalar>(atof(T[4].c_str()));
			normals[i + 2] = static_cast<math::scalar>(atof(T[5].c_str()));
		}
	}
	
	// Krawêdzie.
	utl::uint32_t countidx = 0;
	utl::uint32_t idx = 0;
	
	indices.reserve(vmax);		
	for(utl::uint32_t i = 0; i < faces; ++i)
	{
		std::getline(in, line);
		T.tokenize(line);

		countidx = atoi(T[0].c_str());		
		
		if (countidx == 4)
		{
			indices.push_back(atoi(T[1].c_str()));		
			indices.push_back(atoi(T[2].c_str()));		
			indices.push_back(atoi(T[3].c_str()));		
			
			indices.push_back(atoi(T[3].c_str()));		
			indices.push_back(atoi(T[4].c_str()));		
			indices.push_back(atoi(T[1].c_str()));		
		}
		else	
		{	
			indices.push_back(atoi(T[1].c_str()));		
			indices.push_back(atoi(T[2].c_str()));		
			indices.push_back(atoi(T[3].c_str()));		
		}
	}
	
	// Skalowanie.
	if (_scale != math::scalar(1.0))
	{
		for(utl::uint32_t i = 0; i < vertices.size(); i+=3)
		{
			vertices[i]	  *= _scale;
			vertices[i+1] *= _scale;
			vertices[i+2] *= _scale;
		}
	}
	
	// Obliczanie normalnych.	
	if (!nocalcnormal)
	{
		math::vec3 u,v,d;
		for(utl::uint32_t i = 0; i < indices.size(); i+=3)
		{
			u = math::vec3(	vertices[ 3 * indices[i+1] + 0 ] - vertices[ 3 * indices[i] + 0 ],
							vertices[ 3 * indices[i+1] + 1 ] - vertices[ 3 * indices[i] + 1 ],
							vertices[ 3 * indices[i+1] + 2 ] - vertices[ 3 * indices[i] + 2 ]);
			v = math::vec3(	vertices[ 3 * indices[i+2] + 0 ] - vertices[ 3 * indices[i+1] + 0 ],
							vertices[ 3 * indices[i+2] + 1 ] - vertices[ 3 * indices[i+1] + 1 ],
							vertices[ 3 * indices[i+2] + 2 ] - vertices[ 3 * indices[i+1] + 2 ]);
			d = u.cross_product(v);
			d.normalize();
			
			if (_inversed_normals) 
				d = -d;
			
			normals[ 3 * indices[i] + 0 ] = d.x;
			normals[ 3 * indices[i] + 1 ] = d.y;
			normals[ 3 * indices[i] + 2 ] = d.z;
			
			normals[ 3 * indices[i+1] + 0 ] = d.x;
			normals[ 3 * indices[i+1] + 1 ] = d.y;
			normals[ 3 * indices[i+1] + 2 ] = d.z;
			
			normals[ 3 * indices[i+2] + 0 ] = d.x;
			normals[ 3 * indices[i+2] + 1 ] = d.y;
			normals[ 3 * indices[i+2] + 2 ] = d.z;						
		}
	}
	
	outputmesh.set_draw_mode(GL_TRIANGLES);	
	
	outputmesh.write_vertices(vertices.size() / 3, RWHAET_FLOAT, RWHAES_3, &vertices[0]);
	outputmesh.write_normals(normals.size() / 3, RWHAET_FLOAT, &normals[0]);
	outputmesh.write_indices(indices.size(), RWHIAET_UINT, &indices[0]);
}