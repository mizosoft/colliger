#pragma once
/*
class Button;

class ButtonEffect
{
public:
	enum EffectType
	{
		HOVER,
		CLICK
	};
public:
	ButtonEffect() = default;
	virtual ~ButtonEffect() = default;

	virtual void Apply(Button& button) = 0;

	virtual void UnApply(Button& button) = 0;
};
*/

class BaseButton;

class ButtonEffect
{
public:
	enum EffectType
	{
		HOVER,
		CLICK
	};
public:
	ButtonEffect() = default;
	virtual ~ButtonEffect() = default;

	virtual void Apply(BaseButton& button) = 0;

	virtual void UnApply(BaseButton& button) = 0;
};
