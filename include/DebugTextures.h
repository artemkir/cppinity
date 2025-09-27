#pragma once

SDL_Texture* CreateDebugTexture(SDL_Renderer* renderer, const unsigned char* pixelData, int width, int height) {
	// Create an SDL surface from pixel data
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
		(void*)pixelData, width, height, 8, width, SDL_PIXELFORMAT_INDEX8);

	if (!surface) {
		SDL_Log("Failed to create surface: %s", SDL_GetError());
		return nullptr;
	}

	// Create a grayscale palette (since icon data looks grayscale)
	SDL_Color palette[256];
	for (int i = 0; i < 256; ++i) {
		palette[i] = { (Uint8)i, (Uint8)i, (Uint8)i, 255 };
	}
	SDL_SetPaletteColors(surface->format->palette, palette, 0, 256);

	// Convert surface to a texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture) {
		SDL_Log("Failed to create texture: %s", SDL_GetError());
	}
	return texture;
}

SDL_Texture* CreateDebugTexture2(SDL_Renderer* renderer, const unsigned char* pixelData, int width, int height) {
	// First, allocate storage for RGBA pixel data
	std::vector<Uint32> rgbaPixels(width * height);

	// Allocate a pixel format for RGBA mapping
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (!format) {
		SDL_Log("Failed to allocate pixel format: %s", SDL_GetError());
		return nullptr;
	}

	// Convert grayscale data to RGBA
	for (int i = 0; i < width * height; ++i) {
		Uint8 value = pixelData[i];
		rgbaPixels[i] = SDL_MapRGBA(format, value, value, value, 255);
	}

	SDL_FreeFormat(format);

	// Create the SDL texture directly with RGBA8888 format
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
	if (!texture) {
		SDL_Log("Failed to create texture: %s", SDL_GetError());
		return nullptr;
	}

	// Upload pixel data to the texture
	if (SDL_UpdateTexture(texture, nullptr, rgbaPixels.data(), width * sizeof(Uint32)) != 0) {
		SDL_Log("Failed to update texture: %s", SDL_GetError());
		SDL_DestroyTexture(texture);
		return nullptr;
	}

	return texture;
}

SDL_Texture* CreateCheckerboardDebugTexture(SDL_Renderer* renderer, int width, int height, int checkerSize) {
	std::vector<Uint32> pixels(width * height);
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (!format) {
		SDL_Log("SDL_AllocFormat failed: %s", SDL_GetError());
		return nullptr;
	}

	// Colors for checkerboard: red and white
	Uint32 color1 = SDL_MapRGBA(format, 255, 0, 0, 255);   // red
	Uint32 color2 = SDL_MapRGBA(format, 255, 255, 255, 255); // white

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			bool isChecker = ((x / checkerSize) + (y / checkerSize)) % 2 == 0;
			pixels[y * width + x] = isChecker ? color1 : color2;
		}
	}

	SDL_FreeFormat(format);

	SDL_Texture* texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		width, height);
	if (!texture) {
		SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
		return nullptr;
	}

	if (SDL_UpdateTexture(texture, nullptr, pixels.data(), width * sizeof(Uint32)) != 0) {
		SDL_Log("SDL_UpdateTexture failed: %s", SDL_GetError());
		SDL_DestroyTexture(texture);
		return nullptr;
	}

	return texture;
}

void RenderDebugTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height) {

	SDL_Rect dstRect = { x, y, width, height };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &dstRect);

	if (SDL_RenderCopy(renderer, texture, nullptr, &dstRect) != 0) {
		SDL_Log("Failed to render debug texture: %s", SDL_GetError());
	}
}
