#include "moviestore.h"
#include <iostream>

// Default constructor
MovieStore::MovieStore( )
{
}

/* A function inserts the parameter Movie object into all three BST indices. Note that actual is stored
inside some node of primaryIndex as a copy of the parameter of this function and the other indices
are received the address of the location corresponding to that portion of the primaryIndex node
and store this as a pointer variable within their nodes. Consequently indices based on SKey
holds up less space in total. */
void
MovieStore::insert(const Movie & movie)
{
	primaryIndex.insert(movie.getID(),movie);
	Movie *movie3 = &(primaryIndex.find(movie.getID())->data);
	const Movie* movie2 = movie3;
	SKey key(movie);
	secondaryIndex.insert(key,movie2);
	ternaryIndex.insert(key,movie3);
}


// A function removes the Movie with given id from all indices.
void
MovieStore::remove(const std::string & id)
{
	SKey *key;
	if(primaryIndex.find(id))
		key = new SKey(primaryIndex.find(id)->data);
	else
		return;
	ternaryIndex.remove(*key);
	secondaryIndex.remove(*key);
	primaryIndex.remove(id);
	delete key;
}


// A function removes the Movie with given title and director from all indices.
void
MovieStore::remove(const std::string & title,
                  const std::string & director)
{
	std::string id;
	SKey key(title,director);
	if(secondaryIndex.find(key))
		id = secondaryIndex.find(key)->data->getID();
	else
		return;
	ternaryIndex.remove(key);
	secondaryIndex.remove(key);
	primaryIndex.remove(id);
}


// A function removes all movies with given title from all three indices.
void
MovieStore::removeAllMoviesWithTitle(const std::string & title) 
{
	std::string first,last;
	first = "a";
	last = "{";
	SKey key1(title,first);
	SKey key2(title,last);
	std::list<BinarySearchTree<SKey, Movie*, MovieBalanceCondition, TitleComparator>::Node *> ternaryNodes = ternaryIndex.find(key1,key2);
	std::list<std::string> primaryKeys;
	std::list<SKey> secondaryKeys;
	int ternarySize = ternaryNodes.size();
	for(int i=0; i < ternarySize ; i++){
		primaryKeys.push_back(ternaryNodes.front()->data->getID());
		ternaryNodes.push_back(ternaryNodes.front());
		ternaryNodes.pop_front();
	}
	for(int i=0; i < ternarySize ; i++){
		SKey tmp(title,ternaryNodes.front()->data->getDirector());
		secondaryKeys.push_back(tmp);
		ternaryNodes.push_back(ternaryNodes.front());
		ternaryNodes.pop_front();
	}
	while(!primaryKeys.empty()){
		primaryIndex.remove(primaryKeys.front());
		primaryKeys.pop_front();
	}
	while(!secondaryKeys.empty()){
		secondaryIndex.remove(secondaryKeys.front());
		secondaryKeys.pop_front();
	}
	while(!ternaryNodes.empty()){
		ternaryIndex.remove(ternaryNodes.front()->key);
		ternaryNodes.pop_front();
	}
}


// A function updates the status of the movie with given id as true indicating that the movie is available in store.
void
MovieStore::makeAvailable(const std::string & id) 
{
	if(primaryIndex.find(id))
		primaryIndex.find(id)->data.setAvailable();
}


// A function updates the status of the movie with given title and director as false indicating that the movie is not available in store.
void
MovieStore::makeUnavailable(const std::string & title, 
                           const std::string & director)
{
	SKey key(title,director);
	if(ternaryIndex.find(key))
		ternaryIndex.find(key)->data->setUnavailable();
}


// A function updates company fields of all movies with given director.
void
MovieStore::updateCompany(const std::string & director, 
                           const std::string & Company)
{
	std::string first,last;
	first = "a";
	last = "z";
	SKey key1(first,director);
	SKey key2(last,director);
	std::list<BinarySearchTree<SKey, const Movie*, MovieBalanceCondition, DirectorComparator>::Node *> ranges = secondaryIndex.find(key1,key2);
	while(!ranges.empty()){
		ternaryIndex.find(ranges.front()->key)->data->setCompany(Company);
		ranges.pop_front();
	}
}


/* A function prints each Movie object whose id member falls within the closed interval of the first and the second
parameter of this function onto the console if they were produced no earlier than the third parameter of the function 
whose default value is 0. */
void
MovieStore::printMoviesWithID(const std::string & id1,
                              const std::string & id2,
                              unsigned short since) const 
{
	std::list<BinarySearchTree<std::string, Movie, MovieBalanceCondition>::Node *> ranges = primaryIndex.find(id1,id2);
	while(!ranges.empty()){
		if(ranges.front()->data.getYear() >= since){
			std::cout << ranges.front()->data << std::endl;
		}
		ranges.pop_front();
	}
}


/* A function prints each Movie object whose director member is equal to the first parameter and title member
falls within the closed range of the second and the third parameters of the function onto the console. */
void
MovieStore::printMoviesOfDirector(const std::string & director,
                                  const std::string & first,
                                  const std::string & last) const 
{
	SKey key1(first,director);
	SKey key2(last,director);
	std::list<BinarySearchTree<SKey, const Movie*, MovieBalanceCondition, DirectorComparator>::Node *> ranges = secondaryIndex.find(key1,key2);
	while(!ranges.empty()){
		std::cout << *(ranges.front()->data) << std::endl;
		ranges.pop_front();
	}
}


// A function prints primaryIndex sorted.
void  
MovieStore::printPrimarySorted( ) const
{
  printPrimarySorted(primaryIndex.getRoot());
}

// A recursive utility function to print primaryIndex sorted.
void  
MovieStore::printPrimarySorted(MSTP::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printPrimarySorted(t->left);
  std::cout << t->data << std::endl;
  printPrimarySorted(t->right);
}

// A function prints secondaryIndex sorted.
void  
MovieStore::printSecondarySorted( ) const
{
	
  printSecondarySorted(secondaryIndex.getRoot());
}

// A recursive utility function to print secondaryIndex sorted.
void  
MovieStore::printSecondarySorted(MSTS::Node * t) const
{

 if (t == NULL)
  {
    return;
  }

  printSecondarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printSecondarySorted(t->right);
}

// A function prints ternaryIndex sorted.
void  
MovieStore::printTernarySorted( ) const
{
  printTernarySorted(ternaryIndex.getRoot());
}

// A recursive utility function to print ternaryIndex sorted.
void  
MovieStore::printTernarySorted(MSTT::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printTernarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printTernarySorted(t->right);
}

