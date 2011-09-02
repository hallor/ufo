#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

namespace exceptions
{

class base : public std::exception
{
public:
	base(const char * msg) : m_msg(msg) {}

	virtual const char * what() const throw()
	{
		return m_msg;
	}

private:
	const char * m_msg;
};

class io : public base
{
public:
	io() : base("Stream Access Exception") {}
};

class invalid_resource : public base
{
public:
	invalid_resource() : base("Resource is not valid") {}
};

class invalid_arguments : public base
{
	public:
	invalid_arguments() : base("Invalid arguments passed") {}
};

class load_resource : public base
{
public:
	load_resource() : base("Load resource failed") {}
};

}

#endif // EXCEPTIONS_H
