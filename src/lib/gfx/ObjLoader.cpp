#include "ObjLoader.h"

#include <assert.h>
#include <fstream>
#include <sstream>
#include <common/Debug.h>

namespace funk
{

ObjLoader::ObjLoader() : m_swizzleZY(false), m_bGenNormals(false)
{
}

void ObjLoader::LoadFile( const char * fileName )
{
	std::ifstream file( fileName, std::ios::binary );

	CHECK( file.is_open(), "Cannot open file '%s'\n", fileName );

	std::string dataType;
	dataType.reserve(64);

	while ( file.good() )
	{
		char buffer[128];
		memset(buffer,0,sizeof(buffer));
		dataType = "";

		file.getline( buffer, sizeof(buffer) );

		std::istringstream ss(buffer);
		
		ss >> dataType;

		if ( dataType == "v") // pos
		{
			v3 v;
			ss >> v.x;

			if ( m_swizzleZY ) ss >> v.z >> v.y;
			else ss >> v.y >> v.z;

			m_vertPos.push_back( v );
		}

		else if ( dataType == "vn") // normal
		{
			v3 v;
			ss >> v.x;
			if ( m_swizzleZY ) ss >> v.z >> v.y;
			else ss >> v.y >> v.z;
			m_vertNormal.push_back( v );
		}

		else if ( dataType == "vt") // uv
		{
			v2 v;
			ss >> v.x;
			ss >> v.y;
			m_vertUV.push_back( v );
		}

		else if ( dataType == "f" )
		{
			Face face;
			char c;

			for ( int i = 0; i < Face::kMaxPts && ss.good(); ++i )
			{
				ss >> face.pos[i] >> c;

				if ( isdigit(c) )
				{
					ss.putback(c);
					continue;
				}
				if ( c == '/' )
				{
					ss >> c;

					// read UV
					if ( isdigit(c) )
					{
						ss.putback(c);
						ss >> face.uv[i];
					}
					else ss >> face.normal[i];
				}

				ss >> c;

				if ( c == '/' ) ss >> face.normal[i];
				else if ( isdigit(c) )
				{
					ss.putback(c);
				}
			}

			m_faces.push_back( face );
		}
	}

	file.close();
}

StrongHandle<Vbo> ObjLoader::CreateVbo()
{
	CHECK( m_faces.size() > 0, "There are no faces in the OBJ file!" );

	VertexAttribute * vertAttrib = new VertexAttribute;
	vertAttrib->AddAttrib( VertexAttribute::ATTRIB_VERTEX, VertexAttribute::ATTRIB_FLOAT, 3, sizeof( VertPosNormalUV ), 0 );
	vertAttrib->AddAttrib( VertexAttribute::ATTRIB_NORMAL, VertexAttribute::ATTRIB_FLOAT, 3, sizeof( VertPosNormalUV ), offsetof( VertPosNormalUV, normal ) );
	vertAttrib->AddAttrib( VertexAttribute::ATTRIB_TEXTURE_0, VertexAttribute::ATTRIB_FLOAT, 2, sizeof( VertPosNormalUV ), offsetof( VertPosNormalUV, uv ) );

	std::vector<VertPosNormalUV> data;
	GetVboVertPos(data);
	return new Vbo( (unsigned char*)&data[0], data.size(), sizeof(VertPosNormalUV), vertAttrib );

}

void ObjLoader::GetVboVertPos( std::vector<VertPosNormalUV> & out ) const
{
	out.clear();
	out.reserve( m_faces.size() * Face::kMaxPts );

	for ( size_t iFace = 0; iFace < m_faces.size(); ++iFace )
	{
		const Face & face = m_faces[iFace];

		for( int i = 0; i < Face::kMaxPts; ++i )
		{
			if ( face.pos[i] == -1 ) break;

			if ( i > 2 )
			{
				out.push_back( out[out.size()-3] );
				out.push_back( out[out.size()-2] );						
			}

			int iPos = face.pos[i] - 1;
			int iNormal = face.normal[i] - 1;
			int iUV = face.uv[i] - 1;
			
			VertPosNormalUV v;

			v.pos = m_vertPos[ iPos ];
			if ( iNormal >= 0 ) v.normal = m_vertNormal[ iNormal ];
			if ( iUV >= 0 ) v.uv = m_vertUV[ iUV ];

			out.push_back(v);
		}
	}

	// generaet normals
	if ( m_bGenNormals )
	{
		for ( size_t i = 0; i < out.size(); i += 3 )
		{
			v3 normal = facenormal_cw( out[i].pos, out[i+1].pos, out[i+2].pos );
			out[i].normal = out[i+1].normal = out[i+2].normal = normal;
		}
	}
}
}