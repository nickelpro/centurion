#pragma once
#include <SDL.h>
#include "centurion.h"
#include "font.h"
#include "color.h"
#include <memory>
#include <string>

class centurion::visuals::Graphics {

private:
	friend class centurion::Window; //FIXME
	friend class centurion::visuals::Image; //FIXME

	SDL_Renderer* renderer = nullptr;
	std::shared_ptr<centurion::Font> font;
	centurion::visuals::Color color = Color::WHITE;

	void Update();
	
	void Clear();

	void Render(centurion::visuals::Image& img, centurion::geo::Rectangle rect);

	void Render(centurion::visuals::Image& img, int x, int y, int w, int h);

	void Render(centurion::visuals::Image& img, int x, int y);

	void Render(SDL_Texture* texture, int x, int y, int w, int h);

	void Render(SDL_Texture* texture, int x, int y);

	void RenderFilledRect(int x, int y, int w, int h);

	void RenderOutlinedRect(int x, int y, int w, int h);
	
	void RenderLine(int x1, int y1, int x2, int y2);

	void RenderLine(centurion::geo::Point p1, centurion::geo::Point p2);

	void RenderText(const std::string& text, int x, int y, int w, int h); //FIXME

	void SetFont(const std::shared_ptr<centurion::Font>& font);

	void SetColor(centurion::visuals::Color color);

	void UpdateColor();

	void CheckRenderDimensions(int width, int height);
	
public:
	Graphics(SDL_Renderer* renderer);

	~Graphics();
};