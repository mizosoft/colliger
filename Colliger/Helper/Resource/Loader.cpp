#include "Loader.h"

LoadException::LoadException(const std::string& fileName, const std::string& reason,
	std::exception_ptr cause) noexcept :
	fileName(fileName),
	reason(reason),
	cause(cause)
{
}

const std::string& LoadException::GetFilName() const noexcept
{
	return fileName;
}

const std::string& LoadException::GetReason() const noexcept
{
	return reason;
}

std::string LoadException::GetMessage() const noexcept
{
	return GetMessage(*this);
}

std::exception_ptr LoadException::GetCause() const noexcept
{
	return cause;
}

std::string LoadException::GetMessage(const LoadException& cause)
{
	return "Resource[" + cause.GetFilName() + "] couldn't be loaded" +
		((cause.GetMessage().size() > 0) ? "reason: " + cause.GetMessage() : "");
}
