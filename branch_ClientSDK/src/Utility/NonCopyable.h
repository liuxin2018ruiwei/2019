#ifndef NON_COPYABLE_H__
#define NON_COPYABLE_H__

class NonCopyable 
{  
protected:  
	NonCopyable() {}  
	~NonCopyable() {}  
private:  
	NonCopyable( const NonCopyable& );  
	const NonCopyable& operator=( const NonCopyable& );  
  };  

#endif

