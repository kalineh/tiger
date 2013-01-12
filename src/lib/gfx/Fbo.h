#ifndef _INCLUDE_FBO_H_
#define _INCLUDE_FBO_H_

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include <gfx/TextureParams.h>
#include <math/v2i.h>

#include <vector>

namespace funk
{
	// fwd decl
	class Texture;

	class Fbo : public HandledObj<Fbo>
	{
	public:
		Fbo( int width, int height );
		~Fbo();

		// dimen
		int Width() const { return m_dimen.x; }
		int Height() const { return m_dimen.y; }
		v2i Dimen() const { return m_dimen; }

		void Begin();
		void End();

		// generates default render targets
		// (or you can call SetRenderTargetTex(tex) to bind your own tex
		void GenerateDepthBuffer();
		void GenerateRenderTargetTex( int slot = 0, TexParams texParams = TexParams() );

		// render target slots
		void RemoveRenderTargetTex( int slot );
		bool IsSlotValid( int slot ) const { return m_rtTextures[slot].IsNull(); }
		void SetRenderTargetTex( StrongHandle<Texture> tex, int slot = 0 );
		StrongHandle<Texture> GetTex( int slot = 0 ) const { return m_rtTextures[slot]; }

		GM_BIND_TYPEID(Fbo);
		
	private:

		bool m_boundAsTarget; // bound as render target
		v2i m_dimen;

		// GL ids
		unsigned int m_fboId;
		unsigned int m_fboDepthId;

		void BindFrameBuffer();
		void UnbindFrameBuffer();

		static const int MAX_NUM_SLOTS = 8;
		std::vector< StrongHandle<Texture> > m_rtTextures;
	};

	GM_BIND_DECL(Fbo);
}

#endif