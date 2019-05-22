#pragma once

class Updatable
{
public:
	virtual ~Updatable() = default;
protected:
	virtual void Update(float deltaTime) = 0;
};
