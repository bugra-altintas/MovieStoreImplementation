# MovieStoreImplementation
A movie store implementation by using binary search tree.

## Binary Search Tree Part
The generic **binary search tree (BST)** used in this implementation is implemented as the class
template **BinarySearchTree** with template arguments **Key**, **Object**, **BalanceCondition** and **Comparator** having the default values of **DefaultBalanceCondition**, and **std::less<Key>** defined in <functional>, respectively.

The binary **Node** structure defined in *bst.h* constitutes our BST. It is placed under public section, so all of its functions and data are accessible publicly as **BinarySearchTree<K,O,B,C>::Node**, where **K, O, B, and C** are concrete types instantiating the BST template. The **Node** structure has a **Key** type variable uniquely identifying the node, an **Object** type data to be stored within the node, two pointers to **left** and **right** subtrees, an additional **height** attribute to record the height of the node, and finally a variable
with identifier **subsize** that holds the number of the nodes the node roots including itself; all of which can be initialized by the declared constructor which has been implemented in lines following the type declaration of **BinarySearchTree**.

The data members of the tree is placed under the **private** section where we have the desig-
nated **root pointer** of the tree through which rest of the nodes can be accessed, a variable to store the **number of nodes** in the tree, a **Comparator** type function object to compare different keys, and a **BalanceCondition** type object to indicate whether the BST requires height-balancing following changes to its content through operations such as insert and remove.

The **Comparator** type data member **isLessThan** to compare two **Key** objects **key1** and **key2** by making a call as **isLessThan(key1, key2)** that returns a bool value. The default
value of Comparator template argument is **std::less<Key>** which internally translates calls of the format **isLessThan(k1, k2)** into **operator<(k1, k2)**. 

Similar to the **Comparator**, another function object type called **BalanceCondition** is used
to compare a BST t's actual height with its ideal height, by making a call as  **isBalanced(current_height,ideal_height)**, via overloaded **operator()** to indicate whether the BST is considered as balanced or not. The default version of the template **BalanceCondition** always returns true, which means there is no balance condition.

Constructor and public interface methods are implemented in *bst.h* file. Explanation of them can be found there as well.

Definiton of a complete tree: geeksforgeeks.org/binary-tree-set-3-types-of-binary-tree/

Conversion of a BST of size 5 into a complete BST:
![Complete tree](/figures/conversion_to_complete_tree.png)

Insertion example when the parameter key does not exist within the tree:
![Insertion example](/figures/figure2.png)

Removal of a node having two children:
![Removal example](/figures/figure3.png)
Removal of a node having two children
![Removal example](/figure3.png)

## - Movie Store Part
**MovieStore** constitutes an application relying on the BST.

**Movie** class represents the information needed to be stored for each item in the movie store. A **Movie** includes an **id**, **title**, **director**, **company**, production **year**, **duration** in minutes and **status** to indicate whether the movie is in stock or not. Each movie is uniquely identified by its **id**. There are no two movies with the same **title** and **director**. Unique **title** and **director** pair is also regrouped into **Movie::SecondaryKey** class type and variables of this type are used in building secondary index trees. For details of **Movie** class, inspecting *movie.h* would be nice. Some basic methods of this class can be found in *movie.cpp*.

Implementation of **TitleComparator** type that intends to compare two **Movie::SecondaryKey** objects primarily based on their **titles** is completed by coding only the inline **operator()(constMovie::SecondaryKey &, const Movie::SecondaryKey &) const** member function with respect
to some specifications in *title_comparator.h* file. It performs a **case-insensitive**, **lexicographic** comparison between the **titles** of two key arguments. If the first **title** comes before the second, it returns **true**. If the two **titles** are equal, then it compares the two **directors** and returns **true** only if the first director comes before the second. Otherwise, it returns **false**.
Implementation of **DirectorComparator** type that intends to compare two **Movie::SecondaryKey** objects primarily based on their **directors**. The details are same as **TitleComparator** except for what they primarily based on.

**MovieStore** class includes three **BinarySearchTree** indices, **primaryIndex**,
**secondaryIndex** and **ternaryIndex**. Their types are aliased with shorter names using **typedef** under the **private** section. **primaryIndex** of shorter type name **MSTP** stores actual Movie objects and uses the default lexicographic ordering of **id** values to build itself and yet **secondaryIndex** of new type name **MSTS** and **ternaryIndex** of new type name **MSTT** are both rely on **secondary key** objects of new type name **SKey** values, and these values are ordered utilizing **DirectorComparator** and **TitleComparator** classes, respectively.
Balance condition for all these trees are implemented in **MovieBalanceCondition** class in *moviestore.h* file. There are several methods that **MovieStore** class has. Implementation and explanations of them can be found in *moviestore.cpp*. Inspecting the *moviestore.h*, *moviestore.cpp* would be nice for the detail of class.
Balance condition for all these trees are implemented in **MovieBalanceCondition** class in *moviestore.h* file. There are several methods that **MovieStore** class has. Implementation and explanations of them can be found in *moviestore.cpp*. Inspecting the *moviestore.h*, *moviestore.cpp* would be nice for the detail of class.
