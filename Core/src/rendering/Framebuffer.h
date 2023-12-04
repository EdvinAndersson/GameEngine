#pragma once

#include <stdio.h>
#include "../GLFunc.h"
#include "Texture.h"

namespace GL {

	class Framebuffer {
	public:
		Framebuffer(int width, int height) {
			ReCreate(width, height);
		}
		void ReCreate(int width, int height);
		inline void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer); }
		inline void UnBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		const inline Texture GetTexture() { return texture; }
	private:
		Texture texture;
		unsigned int frame_buffer;
	};

}