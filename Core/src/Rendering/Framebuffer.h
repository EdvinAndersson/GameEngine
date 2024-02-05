#pragma once

#include <stdio.h>
#include "../GLFunc.h"
#include "Texture.h"

namespace CW {

	enum FramebufferType {
		DEFUALT = 0,
		DEPTH = 1
	};

	class Framebuffer {
	public:
		Framebuffer(FramebufferType type, int width, int height) {
			this->type = type;
			ReCreate(width, height);
		}
		void ReCreate(int width, int height);
		inline void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer); }
		inline void UnBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		inline Texture GetTexture() { return texture; }
	private:
		Texture texture;
		unsigned int frame_buffer;
		FramebufferType type;
	};
}