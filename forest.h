#ifndef FOREST_
#define FOREST_

#include "tree.h"
#include "matrix.h"
#include "node.h"


class Forest{

    friend class Tree;
  
  public:
     // Methods for insertions
     void PushBack(Tree* tree); // this method allows us to add a Tree at the end. It takes a Tree as parameter.

		// Constructors
    Forest();
    Forest(const Forest&)=delete;  

        // Destructors
    ~Forest()=default; 
	
        // getter
    int nb_elmts(); // return the value of the attribut nb_elmts_ of the list.
    void show();
    Tree * lastTree();
    Tree * firstTree();
    
  
  protected:
    Tree* firstTree_;
    Tree* lastTree_; 
    int nb_elmts_;
};
#endif // FOREST_
