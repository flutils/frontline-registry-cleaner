#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class noncopyable
{
protected:
	noncopyable(void) {}
	~noncopyable(void) {}

private:
	noncopyable(const noncopyable &);
	const noncopyable & operator=(const noncopyable &);
};

#endif
