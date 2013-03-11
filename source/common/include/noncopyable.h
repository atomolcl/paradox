#ifndef NONCOPABLE_H
#define NONCOPABLE_H

namespace paradox {
	class noncopyable {
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:
		noncopyable(const noncopyable&); 
		const noncopyable& operator=(const noncopyable&);
	};
}
#endif