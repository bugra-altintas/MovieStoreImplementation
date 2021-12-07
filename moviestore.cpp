#include "moviestore.h"
#include <iostream>

MovieStore::MovieStore( ) //implemented, do not change
{
}


// IMPLEMENT
void
MovieStore::insert(const Movie & movie)//DONE 
{
	primaryIndex.insert(movie.getID(),movie);
	Movie *movie3 = &(primaryIndex.find(movie.getID())->data);
	const Movie* movie2 = movie3;
	SKey key(movie);
	//std::cout << "Primary tree:" << std::endl;,
	//printPrimarySorted();
	//std::cout << "INSERTING IN SECONDARY: " << std::endl;
	secondaryIndex.insert(key,movie2);
	//std::cout << "Secondary tree:" << std::endl;
	//printSecondarySorted();
	//std::cout << "INSERTING IN TERNARY: " << std::endl;
	ternaryIndex.insert(key,movie3);
	//std::cout << "Ternary tree:" << std::endl;
	//printTernarySorted();
	//std::cout << "***************************************************************" << std::endl;
	
}


// IMPLEMENT
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


// IMPLEMENT
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


// IMPLEMENT
void
MovieStore::removeAllMoviesWithTitle(const std::string & title) //DONE
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


// IMPLEMENT
void
MovieStore::makeAvailable(const std::string & id) //DONE
{
	if(primaryIndex.find(id))
		primaryIndex.find(id)->data.setAvailable();
}


// IMPLEMENT
void
MovieStore::makeUnavailable(const std::string & title, //DONE
                           const std::string & director)
{
	SKey key(title,director);
	if(ternaryIndex.find(key))
		ternaryIndex.find(key)->data->setUnavailable();
}


// IMPLEMENT
void
MovieStore::updateCompany(const std::string & director, //DONE
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


// IMPLEMENT
void
MovieStore::printMoviesWithID(const std::string & id1,
                              const std::string & id2,
                              unsigned short since) const //DONE
{
	std::list<BinarySearchTree<std::string, Movie, MovieBalanceCondition>::Node *> ranges = primaryIndex.find(id1,id2);
	while(!ranges.empty()){
		if(ranges.front()->data.getYear() >= since){
			std::cout << ranges.front()->data << std::endl;
		}
		ranges.pop_front();
	}
}


// IMPLEMENT
void
MovieStore::printMoviesOfDirector(const std::string & director,
                                  const std::string & first,
                                  const std::string & last) const //DONE
{
	SKey key1(first,director);
	SKey key2(last,director);
	std::list<BinarySearchTree<SKey, const Movie*, MovieBalanceCondition, DirectorComparator>::Node *> ranges = secondaryIndex.find(key1,key2);
	while(!ranges.empty()){
		std::cout << *(ranges.front()->data) << std::endl;
		ranges.pop_front();
	}
}



void  // implemented, do not change
MovieStore::printPrimarySorted( ) const
{
  printPrimarySorted(primaryIndex.getRoot());
}


void  // implemented, do not change
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


void  // implemented, do not change
MovieStore::printSecondarySorted( ) const
{
	
  printSecondarySorted(secondaryIndex.getRoot());
}


void  // implemented, do not change
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


void  // implemented, do not change
MovieStore::printTernarySorted( ) const
{
  printTernarySorted(ternaryIndex.getRoot());
}


void  // implemented, do not change
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

