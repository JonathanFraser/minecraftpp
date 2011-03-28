#ifndef __REFCOUNT_H
#define __REFCOUNT_H
/**
*	@class Countee
*	@brief base class that counts its interfaces
*
*	This class along with the Counter base class implements 
* a form of reference counting
*/
class Countee {
	private:
		unsigned int count;
		friend class Counter;
		void reference();
		void dereference();
	protected:
		Countee();	
		virtual void freeze() = 0;
		virtual void thaw() = 0;
	public:
		unsigned int getCount();
};

/**
*	@class Counter
*	@brief the base class for the interface class to be counted 
*
**/
class Counter {
	protected:
		
		Countee* data;
	public:
		Counter(Countee*);
		Counter(const Counter&);
		virtual ~Counter();
		unsigned int getCount();
};

#endif
