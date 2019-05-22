#pragma once

/// A deleted copy ctor/ass version of sf::NonCopyable
/// which might let the compiler report better feedback
/// on accidental copy creation
class NoCopy 
{
protected:
	NoCopy() = default;
	virtual ~NoCopy() = default;
private:
	NoCopy(NoCopy& other) = delete;

	NoCopy operator=(NoCopy& other) = delete;
};
