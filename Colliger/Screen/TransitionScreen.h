#pragma once

#include <memory>
#include <functional>
#include <future>
#include <atomic>

#include "SFML/Graphics/RectangleShape.hpp"
#include "Screen.h"
#include "Helper/Concurrent/ThreadWorker.h"
#include "Helper/Graphics/FadeAlpha.h"

class TransitionScreen :
	public Screen
{
private:
	using ScreenPtr = std::shared_ptr<Screen>;
	using ScreenFuture = std::shared_future<ScreenPtr>;
public:
	using ScreenFactory = std::function<ScreenPtr(void)>;
private:
	class FactoryTask
	{
	public:
		FactoryTask(const ScreenFactory& screenFactory);
		~FactoryTask() = default;

		FactoryTask(const FactoryTask& other);
		FactoryTask& operator=(const FactoryTask& other);

		FactoryTask(FactoryTask&& other) noexcept;
		FactoryTask& operator=(FactoryTask&& other) noexcept;

		void operator()();

		ScreenFuture GetFuture() const;
	private:
		void CopyFrom(const FactoryTask& other);

		void MoveFrom(FactoryTask& other);
	public:
		std::atomic<bool> started;
	private:
		ScreenFactory screenFactory;
		std::shared_ptr<std::promise<ScreenPtr>> promisePtr;
	};

	enum RenderState
	{
		PREV_SCREEN,
		NEXT_SCREEN
	};
public:
	TransitionScreen(Game& game, const ScreenFactory& nextScreenFactory);
	~TransitionScreen() = default;

	virtual void Resize(const sf::Vector2u& newSize) override;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	bool ScreenFutureReady() const; // Convenience method for getting nextScreenFuture status
private:
	static const float ALPHA_INCREASE_RATE;
private:
	ScreenPtr previousScreen;
	ScreenFuture nextScreenFuture;
	FadeAlpha fadeIn, fadeOut;
	RenderState renderState;
	sf::Color fadeColor;
	sf::RectangleShape rect;
	ThreadWorker factoryThread;
};
