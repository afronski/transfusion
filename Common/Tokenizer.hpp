#ifndef __COMMON_TOKENIZER_HPP__
#define __COMMON_TOKENIZER_HPP__

namespace utl
{
	//--------------------------------------------------------------------------------------------
	// Tokenizacja.
	//--------------------------------------------------------------------------------------------			
	template <char separator>
		class tokenizer
		{
			public:
				typedef std::vector<char>::size_type size_type;	   

				void tokenize(const std::string& str)
				{
					this->clear();
					_data = str;
					
					std::string::size_type begin, end = 0;
					while ((begin = _data.find_first_not_of(separator, end)) != std::string::npos) 
					{
						end = _data.find(separator, begin);
						_indices.push_back(index_type(begin, end - begin));
					}
				}

				size_type size() const 
				{ 
					return _indices.size();	
				}
	      
				std::string operator[](size_type idx) const 
				{
					return _data.substr(_indices[idx].first, _indices[idx].second);
				}
				
				void clear()
				{					
					_indices.clear();
				}
				
			private:
				typedef std::pair<std::string::size_type, std::string::size_type> index_type;
				typedef std::vector<index_type> indices_type;

				indices_type _indices;
				std::string _data;				
		}; 			

} // end namespace utl

#endif