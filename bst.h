#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <functional>
#include <cstddef>
#include <ostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <utility>

#include <iostream>


class DefaultBalanceCondition   // default BST
{
  public:
    bool operator() (int current_height, int ideal_height) const
    {
      return true;
    }
};


template <typename Key, typename Object,
          typename BalanceCondition=DefaultBalanceCondition,
	  typename Comparator=std::less<Key> >
class BinarySearchTree
{
  public:  
    struct Node   // core structure of BinarySearchTree
    {
       Key key;         // unique key
       Object data;     // data to be stored, characterized by key
       Node * left;     // pointer to left subtree
       Node * right;    // pointer to right subtree
       size_t height;   // height of the node
       size_t subsize;  // size of the subtree node roots including itself

       Node(const Key &, const Object &, 
            Node *, Node *, size_t =0, size_t =1);   // Node constructor
    };

  public:  
    BinarySearchTree();  // zero-parameter constructor
    BinarySearchTree(const std::list<std::pair<Key, Object> > &); // list is always sorted!
    ~BinarySearchTree();  // destructor

  public:  
    void insert(const Key &, const Object &);  // insert new Key-Object
    void remove(const Key &);  // remove Node characterized by Key
    void toCompleteBST();  // convert into a complete BST

  public:  
    Node * find(const Key &) const; // single item
    std::list<Node *> find(const Key &, const Key &) const; // range queries
    int height() const;  // return the height of the tree
    size_t size() const; // return the number of items in the tree
    bool empty() const;  //return whether the tree is empty or not
    Node * getRoot() const; // return a pointer to the root of the tree
    void print(std::ostream &) const;  // print tree structure into an output stream
	

  private:  
    Node * root;                     // designated root
    size_t numNodes;                 // size
    Comparator isLessThan;           // operator upon which BST nodes are arranged
    BalanceCondition isBalanced;     // signals whether the signal is balanced
  
  private:  //utilizations
    Node * find(const Key &, Node *) const;
    int height(Node *) const;
    int subsize(Node *) const;
    void print(Node *, std::ostream &) const;
	void find(const Key &, const Key &, Node *, std::list <Node*> &) const;
	bool isLeaf(Node*) const;
    void makeEmpty(Node * &);       // utility for destructor
	void toCompleteBST(Node *&,std::vector<Node*> &);
	void inorder_vector(Node* , std::vector<Node*> &);
	void readTree(Node*&,int,std::list<std::pair<Key,Object>> &items);
	void insert(const Key &, const Object &,Node *&);
	void remove(const Key &,Node *&);
	void move(Node *&,Node*&);
	int findMid(int);

  private:  // static utility functions
    template <typename T> //static utility function
    static const T & max(const T &, const T &);

};

#endif


// Node constructor
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::Node::
Node(const K & _k, const O & _d, Node * _l, Node * _r, size_t _h, size_t _s)
  : key(_k), data(_d), left(_l), right(_r), height(_h), subsize(_s)
{
}

// zero-parameter constructor
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree()
  : root(NULL), numNodes(0)  
{
}

/* Given that the key object pairs in the list argument are sorted in ascending order with respect to
isLessThan function object provided as a class template argument whose default is the ordinary operator< 
on Key values; this one-argument constructor builds up a complete tree. It constructs a minimum-height
binary search tree that is complete from given sorted data list.
*/
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::BinarySearchTree(const std::list<std::pair<K,O> > & datalist)
   : root(NULL), numNodes(0)  // change it as you'd like
{
	numNodes = datalist.size();
	std::list<std::pair<K,O>> items(datalist);
	readTree(root,numNodes,items);
	items.clear();
	items.resize(0);
}

// A utility function constructs the BST by finding root's place for the root and internal nodes, recursively.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::readTree(Node*&t, int n,std::list<std::pair<K,O> > &items){
	if(n>0){
		int mid = findMid(n); // find root's place
		t = new Node(NULL,NULL,NULL,NULL,0,0); //root
		readTree(t->left,mid,items); // construct the left sub-tree
		t->key = items.front().first;
		t->data = items.front().second;
		items.pop_front();
		readTree(t->right,(n-mid-1),items); // construct the right sub-tree
		t->height = 1 + max(height(t->left),height(t->right));
		t->subsize = 1 + subsize(t->right) + subsize(t->left);
	}
}

// A utility function finds the root's place of a complete tree with given size of tree.
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::findMid(int n){
	int level = int(log2(n)) + 1;
	int mid;
	int possible_left,possible_right;
	possible_left = pow(2,level-1) - 1;
	possible_right = pow(2,level-2) - 1;
	if((possible_right + possible_left) > (n-1)){
		
		mid = (n-1) - possible_right;
	}
	else{
		mid = possible_left;
	}
	return mid;
}


/* A function converts the current tree to a minimum-height complete BST. Conversion of a BST of
size 5 into a complete BST is shown in Figure 1 in README.md file.
*/ 
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::toCompleteBST()
{
	std::vector<Node*> tmp;
	inorder_vector(root,tmp);
	return toCompleteBST(root,tmp); //utility function
	
}

// A utility recursive function that converts given nodes vector to a complete BST.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::toCompleteBST(Node *&t,std::vector<Node*> &nodes){
	if(nodes.size() == 0){
		return;
	}
	
	if(nodes.size() == 1){
		return;
	}
	int mid = findMid(nodes.size()); 
	Node* tmp;
	tmp = nodes.at(mid);
	std::vector<Node*> nodes_left = std::vector<Node*>(nodes.begin(),nodes.begin() + mid);
	std::vector<Node*> nodes_right = std::vector<Node*>(nodes.begin() + mid + 1,nodes.end());
	if(nodes_left.size() == 0){
		tmp->left = NULL;
	}
	if(nodes_left.size() == 1){
		tmp->left = nodes_left.at(0);
		nodes_left.at(0)->left = NULL;
		nodes_left.at(0)->right = NULL;
		nodes_left.at(0)->height = 0;
		nodes_left.at(0)->subsize = 1;
	}
	else{
		toCompleteBST(tmp->left,nodes_left);
	}
	if(nodes_right.size() == 0){
		tmp->right = NULL;
	}
	if(nodes_right.size() == 1){
		tmp->right = nodes_right.at(0);
		nodes_right.at(0)->left = NULL;
		nodes_right.at(0)->right = NULL;
		nodes_right.at(0)->height = 0;
		nodes_right.at(0)->subsize = 1;
	}
	else{
		toCompleteBST(tmp->right,nodes_right);
	}
	t = tmp;
	t->height = 1 + max(height(t->right),height(t->left));
	t->subsize = 1 + subsize(t->left) + subsize(t->right);
}

/* A function adds a node to BST with given Key, Object pair by following the BalanceCondition. 
If Key exists, the function just updates it. Insertion example when the parameter key does not exist
within the tree is shown in Figure 2.*/
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::insert(const K & k, const O & x) //CHECK COMPLEXITY!!
{
	
	if(root == NULL){
		root = new Node(k,x,NULL,NULL,0,1);
		return;
	}
	Node* update = find(k); //whether given Key exists or not
	if(update == NULL)
		insert(k,x,root); //utility function
	else
		update->data = x;
}

/* A utility recursive function that creates the node with given Key, Object pair and inserts it to
the right place such that BST conditions hold. Then, the function checks whether the upper part of
tree is balanced still. If the balance is broken, the function rebalances it by converting the tree to
complete tree. 
*/
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::insert(const K & k, const O & x, Node* &t){
	//find where to insert:
	if(isLeaf(t)){
		
		if(isLessThan(k,t->key))
			t->left = new Node(k,x,NULL,NULL,0,1);
		else
			t->right = new Node(k,x,NULL,NULL,0,1);
	}
	else if(t->left != NULL && t->right == NULL){
		if(isLessThan(k,t->key))
			insert(k,x,t->left);
		else
			t->right = new Node(k,x,NULL,NULL,0,1);
	}
	else if(t->right != NULL && t->left == NULL){
		
		if(isLessThan(k,t->key))
			t->left = new Node(k,x,NULL,NULL,0,1);
		else
			insert(k,x,t->right);
	}
	else{
		if(isLessThan(k,t->key))
			insert(k,x,t->left);
		else
			insert(k,x,t->right);
	}
	//update height & subsize:
	t->height = 1 + max(height(t->right),height(t->left));
	t->subsize = 1 + subsize(t->right) + subsize(t->left);
	//control the balance:
	if(t->subsize > 1){
		if(!isBalanced(t->height,log2(t->subsize))){
			std::vector<Node*> tmp;
			inorder_vector(t,tmp);
			toCompleteBST(t,tmp);
		}
	}
}

// A function removes the node with given Key, if it exists.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::remove(const K & k) //CHECK COMPLEXITY!!
{
	if(root == NULL)
		return;
	Node* toDelete = find(k,root);
	if(toDelete == NULL)
		return;
	else
		remove(k,root);//utility function
}

/* A utility recursive function that deletes the node with given Key. It inspects the issue in three cases.
If the node is leaf, it simply deletes the node.
If the node has one child, it sets up pointer connections with the parent and the child of the node and 
delete the node.
If the node has two children, as shown in Figure 3a in README file, following the path marked with orange 
arrows, the function identifies the node q which is the inorder successor of the node to remove p and places
q into where p currently is. Then, it deletes the node pointed by p which is no longer 
accessible via the root as shown in Figure 3b and decrements the number of nodes.
Also, the function checks the path that it has travelled via backtracking and rebalance all visited nodes
according to the BalanceCondition
*/
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::remove(const K & k,Node *&t){
	if(isLessThan(t->key,k)){
		remove(k,t->right);
	}
	else if(isLessThan(k,t->key)){
		remove(k,t->left);
	}
	else{	//Deletion part
		//Case 1: Leaf
		if(isLeaf(t)){
			delete t;
			t = NULL;
			return;
		}
		//Case 2: Node with one child
		else if(t->left != NULL && t->right == NULL){
			Node *tmp = t;
			t = t->left;
			delete tmp;
			return;
		}
		else if(t->right != NULL && t->left == NULL){
			Node *tmp = t;
			t = t->right;
			delete tmp;
			return;
		}
		//Case 3: Node with both child
		else if(t->right != NULL && t->left != NULL){
			//find inorder successor q
			//place q into t
			move(t,t->right);
			remove(k,t->right);
		}
		
		
	}
	//rebalance the tree starting from q's old parent to root
	if(t){
		//update height & subsize
		t->height = 1 + max(height(t->right),height(t->left));
		t->subsize--;
		//control the balance
		if(t->subsize > 1){
			if(!isBalanced(t->height,log2(t->subsize))){
				std::vector<Node*> tmp;
				inorder_vector(t,tmp);
				toCompleteBST(t,tmp);
			}
		}
	}
}

// A utility function moves a given node t to the place of given node q. It does not copy anything, only pointer, height and subsize adjustments.  
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::move(Node *&t, Node *&q){ //CHECK COMPLEXITY!!
	if(q->left == NULL){
		if(q == t->right){
			Node *tmpq = q;
			Node *tmpt = t;
			tmpt->right = tmpq->right;
			tmpq->left = tmpt->left;
			tmpq->right = tmpt;
			t = tmpq;
			tmpt->left = NULL;
			tmpq->right->height = 1 + max(height(tmpq->right->right),height(tmpq->right->left));
			tmpq->right->subsize = 1 + subsize(tmpq->right->right) + subsize(tmpq->right->left);
			tmpq->height = 1 + max(height(tmpq->right),height(tmpq->left));
			tmpq->subsize = 1 + subsize(tmpq->right) + subsize(tmpq->left);
		}
		else if(q->right != NULL){
			Node *tmpq = q;
			Node *tmpt = t;
			Node* tmptr = t->right;
			tmpt->right = tmpq->right;
			tmpq->left = tmpt->left;
			tmpq->right = tmptr;
			tmpt->left = NULL;
			t = tmpq;
			q = tmpt;
			t->height = 1 + max(height(t->right),height(t->left));
			t->subsize = 1 + subsize(t->right) + subsize(t->left);
			q->height = 1 + max(height(q->right),height(q->left));
			q->subsize = 1 + subsize(q->right) + subsize(q->left);
		}
		else if(isLeaf(q)){
			Node *l = t->left;
			Node *r = t->right;
			Node *tmp = t;
			t->right = q->right;
			t->left = q->left;
			q->right = r;
			q->left = l;
			t = q;
			q = tmp;
			t->height = 1 + max(height(t->right),height(t->left));
			t->subsize = 1 + subsize(t->right) + subsize(t->left);
			q->height = 1 + max(height(q->right),height(q->left));
			q->subsize = 1 + subsize(q->right) + subsize(q->left);
		}
	}
	else{
		move(t,q->left);
	}
}

// A function returns the pointer of node that has a Key as given 'key'.
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key) const
{	
  return find(key, root); // utility function
}

// A utilty recursive function returns the pointer of node that has a Key as given 'key' by traversing the BST.
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::find(const K & key, Node * t) const
{
  if (t == NULL)
  {
    return NULL;
  }

  if (isLessThan(key, t->key))
  	return find(key, t->left);
  else if (isLessThan(t->key, key))
    return find(key, t->right);
  else //found
    return t;
}


/*A function returns a C++ STL list of Node * variables that store addresses of the nodes whose Key
values fall into the range between the first and second parameters of the function. It is assumed
that the first Key parameter isLessThan or equal to the second and that the interval is closed.
*/
template <typename K, typename O, typename B, typename C>
std::list<typename BinarySearchTree<K,O,B,C>::Node *>
BinarySearchTree<K,O,B,C>::find(const K & lower, const K & upper) const //CHECK COMPLEXITY!!
{
  std::list<Node *> ranges;
  find(lower,upper,root,ranges); // utility function
  return ranges;
}

// A utility recursive function for the function just above.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::find(const K & lower, const K & upper, Node *t, std::list<Node*> &ranges) const{
	if(t == NULL)
		return;
	
	if(isLessThan(upper,t->key))
		find(lower,upper,t->left,ranges);
	else if(isLessThan(t->key,lower))
		find(lower,upper,t->right,ranges);
	
	else{
		find(lower,upper,t->left,ranges);
		ranges.push_back(t);
		find(lower,upper,t->right,ranges);
	}
	
}

// A utility function returns the root pointer.
template <typename K, typename O, typename B, typename C>
typename BinarySearchTree<K,O,B,C>::Node *
BinarySearchTree<K,O,B,C>::getRoot() const
{
  return root;
}


// A utility function returns the height of the tree.
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height() const
{
  return height(root);
}


// A utility function returns the height of the tree with given Node t.
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::height(Node * t) const
{
  return (t == NULL) ? -1 : t->height;
} 


// A utility function returns the subsize of the tree with given Node t.
template <typename K, typename O, typename B, typename C>
int
BinarySearchTree<K,O,B,C>::subsize(Node * t) const
{
  return (t == NULL) ? 0 : t->subsize;
}

// A utility function returns the number of nodes in the tree.
template <typename K, typename O, typename B, typename C>
size_t
BinarySearchTree<K,O,B,C>::size() const
{
  return numNodes;
}

// A utility function checks whether the tree has any nodes or not
template <typename K, typename O, typename B, typename C>
bool
BinarySearchTree<K,O,B,C>::empty() const
{
  return numNodes == 0;
}

// A utility function empties the tree by deleting all nodes.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::makeEmpty(Node * & t)
{
  if (t != NULL)
  {
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;

    --numNodes;
  }
  
  t = NULL;
}


// Destructor
template <typename K, typename O, typename B, typename C>
BinarySearchTree<K,O,B,C>::~BinarySearchTree()
{
  makeEmpty(root);
}


// A utility function prints the tree.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(std::ostream & out) const
{
  print(root, out);
  out << '\n';
}


// A utility recursive function prints the tree recursively.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::print(Node * t, std::ostream & out) const
{
  if (t != NULL && t->left != NULL) 
  {
    out << '[';
    print( t->left, out );
  }
  else if (t != NULL && t->left == NULL && t->right != NULL)
  {
    out << "[";
  }

  if (t != NULL)
  {
    if (t->left == NULL && t->right == NULL)
    {
      out << '(' << (t->key) << ')';
    }
    else if (t->left != NULL || t->right != NULL)
    {
      out << '{' << (t->key) << ",H" << t->height << ",S" << t->subsize << '}';
    }
  }
  
  if (t != NULL && t->right != NULL)
  {
    print( t->right, out );
    out << ']';
  }
  else if (t != NULL && t->left != NULL && t->right == NULL)
  {
    out << "]";
  }
}

// A utility function checks wheter given node is leaf or not.
template <typename K, typename O, typename B, typename C>
bool
BinarySearchTree<K,O,B,C>::isLeaf(Node* t) const {
	if(t->left == NULL && t->right == NULL){
		return true;
	}
	return false;
}

// A utility recursive function that converts given tree to vector, by inorder traversal.
template <typename K, typename O, typename B, typename C>
void
BinarySearchTree<K,O,B,C>::inorder_vector(Node* t,std::vector<Node*> &nodes){
	if(t==NULL) return;
	inorder_vector(t->left,nodes);
	nodes.push_back(t);
	inorder_vector(t->right,nodes);
}

// A utility function returns the maximum of given two parameters.
template <typename K, typename O, typename B, typename C>
template <typename T>
const T &
BinarySearchTree<K,O,B,C>::max(const T & el1, const T & el2)
{
  return el1 > el2 ? el1 : el2;
}

