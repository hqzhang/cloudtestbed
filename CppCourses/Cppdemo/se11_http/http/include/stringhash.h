#ifndef _STRING_HASH_H_
#define _STRING_HASH_H_
namespace bbt{
struct StringHash
{
	size_t operator()(const std::string &str) const 
	{
		const std::string::size_type size = str.size();
		size_t h = 0; 
		for(std::string::size_type i = 0; i < size; i++) 
		{
			h = 5 * h + str[i];
		}
		return h;
	}
};
};
#endif
