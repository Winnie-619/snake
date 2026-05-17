#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "SnakeGame.h"
#include <chrono>
#include <thread>
#include <random>

SnakeGame::SnakeGame(int width, int height)
: width_(width), height_(height), dir_(RIGHT) {
	snake_.push_back({width / 2, height / 2});
	GenerateFood();
}

void SnakeGame::GenerateFood() {
	static std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> distX(0, width_ - 1);
	std::uniform_int_distribution<int> distY(0, height_ - 1);
	food_ = {distX(rng), distY(rng)};
}

bool SnakeGame::Collision(Position p) {
	for (auto &s : snake_)
		if (s.x == p.x && s.y == p.y) return true;
	return p.x < 0 || p.x >= width_ || p.y < 0 || p.y >= height_;
}

void SnakeGame::Move() {
	Position head = snake_.front();
	switch(dir_) {
		case UP: head.y--; break;
		case DOWN: head.y++; break;
		case LEFT: head.x--; break;
		case RIGHT: head.x++; break;
	}
	
	if (Collision(head)) {
		game_over_ = true;
		return;
	}
	
	snake_.insert(snake_.begin(), head);
	
	if (head.x == food_.x && head.y == food_.y) {
		grow_ = true;
		GenerateFood();
	}
	
	if (!grow_)
		snake_.pop_back();
	else
		grow_ = false;
}

void SnakeGame::Update() {
	Move();
}

void SnakeGame::ChangeDirection(Direction dir) {
	dir_ = dir;
}

ftxui::Element SnakeGame::Render() {
	using namespace ftxui;
	Elements rows;
	for (int y = 0; y < height_; y++) {
		Elements row;
		for (int x = 0; x < width_; x++) {
			if (x == food_.x && y == food_.y)
				row.push(text("🍎"));
			else {
				bool isSnake = false;
				for (auto &s : snake_) if (s.x == x && s.y == y) {isSnake=true; break;}
				row.push(isSnake ? text("🟩") : text("  "));
			}
		}
		rows.push(hbox(row));
	}
	return vbox(rows);
}

int main() {
	using namespace ftxui;
	SnakeGame game(20, 15);
	
	auto screen = ScreenInteractive::TerminalOutput();
	
	Component renderer = Renderer([&] { return game.Render(); });
	
	auto main_component = CatchEvent(renderer, [&](Event e) {
		if (e == Event::ArrowUp) game.ChangeDirection(UP);
		if (e == Event::ArrowDown) game.ChangeDirection(DOWN);
		if (e == Event::ArrowLeft) game.ChangeDirection(LEFT);
		if (e == Event::ArrowRight) game.ChangeDirection(RIGHT);
		if (e == Event::Character(' ')) { /* TODO: speed boost */ }
		return true;
	});
	
	std::thread t([&](){
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			game.Update();
			screen.PostEvent(Event::Custom);
			if (false) break; // TODO: game over check
		}
	});
	
	screen.Loop(main_component);
}
