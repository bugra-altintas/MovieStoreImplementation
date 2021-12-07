#ifndef _title_h__
#define _title_h__

#include "movie.h"
#include <cstring>
#include <iostream>


class TitleComparator 
{
  public:
    bool operator( ) (const Movie::SecondaryKey & key1, 
                      const Movie::SecondaryKey & key2) const
    { 
		
	bool result = false;
	char f1,f2;
	int i=0;
	// title comparison
	i = 0;
	f1 = key1.getTitle()[i];
	f2 = key2.getTitle()[i];
	if(key1.getTitle().length() != key2.getTitle().length()){
		while(f1 != '\0' || f2 != '\0'){
			if(f1 == f2){
				i++;
				f1 = key1.getTitle()[i];
				f2 = key2.getTitle()[i];
			}
			else{
				if(f1 >= 65 && f1 <= 90){
					f1 += 32;
				}
				if(f2 >= 65 && f2 <= 90){
					f2 += 32;
				}
				if(f1 == f2){
					i++;
					f1 = key1.getTitle()[i];
					f2 = key2.getTitle()[i];
				}
				else{
					if(f1 < f2){
						result = true;
					}
					return result;
				}
			}
		}
		if(f1 != f2){
			if(f1 < f2){
				result = true;
			}
			return result;
		}
	}
	else{
		i = 0;
		int size = key1.getTitle().length();
		for(i=0; i < size; i++){
			f1 = key1.getTitle()[i];
			f2 = key2.getTitle()[i];
			if(f1 != f2){
				if(f1 >= 65 && f1 <= 90){
					f1 += 32;
				}
				if(f2 >= 65 && f2 <= 90){
					f2 += 32;
				}
				if(f1 != f2){
					if(f1 < f2){
						result = true;
					}
					return result;
				}
			}
		}
	}
	if(!result){ //director comparison
		int i=0;
		f1 = key1.getDirector()[i];
		f2 = key2.getDirector()[i];
		if(key1.getDirector().length() != key2.getDirector().length()){
			while(f1 != '\0' || f2 != '\0'){
				if(f1 == f2){
					i++;
					f1 = key1.getDirector()[i];
					f2 = key2.getDirector()[i];
				}
				else{
					if(f1 >= 65 && f1 <= 90){
						f1 += 32;
					}
					if(f2 >= 65 && f2 <= 90){
						f2 += 32;
					}
					if(f1 == f2){
						i++;
						f1 = key1.getDirector()[i];
						f2 = key2.getDirector()[i];
					}
					else{
						if(f1 < f2){
							result = true;
						}
						return result;
					}
				}
			}
			if(f1 != f2){
				if(f1 < f2){
					result = true;
				}
				return result;
			}
		}
		else{
			int i = 0;
			int size = key1.getDirector().length();
			for(i=0; i < size; i++){
				f1 = key1.getDirector()[i];
				f2 = key2.getDirector()[i];
				if(f1 != f2){
					if(f1 >= 65 && f1 <= 90){
						f1 += 32;
					}
					if(f2 >= 65 && f2 <= 90){
						f2 += 32;
					}
					if(f1 != f2){
						if(f1 < f2){
							result = true;
						}
						return result;
					}
				}
			}
		}
	}
	return result;
	
	}
};

#endif
