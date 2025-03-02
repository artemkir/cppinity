#pragma once

// SDL Renderer Implementation
class SDLRenderer : public IRenderer {
private:
	SDL_Renderer* renderer;

public:
	SDLRenderer(SDL_Renderer* r) : renderer(r) {}

	void Clear(int r, int g, int b) override {
		SDL_RenderClear(renderer);
	}

	void DrawRect(int x, int y, int w, int h) override {
		SDL_Rect rect = { x, y, w, h };
		SDL_RenderFillRect(renderer, &rect);
	}

	void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override {
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	}

	void Present() override {
		SDL_RenderPresent(renderer);
	}
};