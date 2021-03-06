#include "tree.h"
#include <iostream>
#include "node.h"
#include "matrix.h"


int nulltab[2];

 
//constructor
Tree::Tree(Node * node){
  //Initialisation of a tree
  FirstNode_ = node;
  Fitness_ = 0;
  NbrNode_ = 1;
  nextTree_ = nullptr; 
  generation_ = -1;
}


Tree::Tree( Tree* tree){
  Node * FirstNode_init=tree->FirstNode();
  //Node * node =new Node (tree->FirstNode_);
  //FirstNode_ = node;
  Fitness_ = 0;
  NbrNode_ = tree->NbrNode_;
  nextTree_ = nullptr; 
  generation_ = tree->generation_;
  std::string True="true";
  Node * tmp=new Node(True);
  int i=0;
  copy_nodes(FirstNode_init,tmp,i);
}


void Tree::copy_nodes(Node * node, Node * father, int i){
  if (node->values()=="&&" or node->values()=="||" or node->values()=="!"){
   
    Node * op_node=new Node(node->values(), father);
    
    if (i==0){
      FirstNode_=op_node;
    }
    ++i;
    
    father=op_node;  

    Node * nextnode1_init=node->NextNode1();
    Node * nextnode2_init=node->NextNode2();
      
    if (node->NextNode1() !=nullptr){  
      copy_nodes(nextnode1_init, father,i);
    }
    if (node->NextNode2() !=nullptr){
      copy_nodes(nextnode2_init, father,i);
    }
  } 
  
  else{
    Node * param_node=new Node (node->values(), father); 
    if (i==0){
      FirstNode_=param_node;
    }
    ++i; 
  }
}





void Tree::mutation(){
  
  Node * node=choose();
  if (node->values()=="&&" or node->values()=="||" ){
    int mut_type=rand()%3;
    if (mut_type==0){substitution(node);}
    if (mut_type==1){insertion(node);}
    else{switch_op(node);}
  }
  if ( node->values()=="!"){
    int mut_type=rand()%2;
    if (mut_type==0){substitution(node);}
    if (mut_type==1){insertion(node);}
  }
  else{
    int mut_type=rand()%2;
    if (mut_type==0){substitution(node);}
    if (mut_type==1){insertion(node);}
    //else{switch_param(node);}
  }
}


Node * Tree::choose(){
  cross(FirstNode_);
  return(crossing(FirstNode_));
}


Node * Tree::crossing(Node * node){
  int mutation_rate=rand()%NbrNode_;  //taux de mutation

  if (mutation_rate==0){
    return(node);
  }
  else{
    int left_right=rand()%2;
    if (left_right==0){   // On se déplace vers le NextNode1
      if (node->NextNode1()==nullptr){   //extrémité de l'arbre
        return(crossing(FirstNode_));
      }
      else{  //coeur de l'arbre
        return(crossing(node->NextNode1()));
      }
    }
    else{  //On se déplace vers le NextNode2
      if (node->NextNode2()==nullptr){   //extrémité de l'arbre
        return(crossing(FirstNode_));
      }
      else{  //coeur de l'arbre
        return(crossing(node->NextNode2()));
      }
    }
  }
}


void Tree::switch_op(Node * node){
  std::string ou="||";
  std::string et="&&";
  if (node->values()=="&&"){node->setvalues(ou);}
  if (node->values()=="||"){node->setvalues(et);}
}

void Tree::switch_param(Node * node){
  //srand(time(NULL));
  std::string param[]={"true","false","x1","x2","x3"};
  int i=rand()%5;
  if (node->values()==param[i]){switch_param(node);}
  else {node->setvalues(param[i]);}
  
}


void Tree::insertion(Node * node){
  
  Node * father=node->FatherNode();
  std::string non="!";
  Node * new_node=new Node(non);
  
  if (node==FirstNode_){FirstNode_=new_node;}
  
  if (father!=nullptr and father->NextNode1()==node){
    father->setNextNode(new_node);
  }
  if (father!=nullptr and father->NextNode2()==node){
    father->setNextNode2(new_node);
  }
  
  new_node->setNextNode(node);
  new_node->setFatherNode(father);
  
  node->setFatherNode(new_node);
}


void Tree::substitution(Node * node){
  std::string options[]={"true","false","x1","x2","x3","&&","||","!"};
  //srand(time(NULL));
  int i=rand()%8;   // Choix d'une option

  Node * father=node->FatherNode();
  Node * new_node= new Node(options[i]) ;
  
  if (node==FirstNode_){FirstNode_=new_node;}
  
  if (options[i]=="&&" or options[i]=="||"){
  
    int param1=rand()%5;  //choix d'un paramètre
    int param2=rand()%5;  //choix d'un paramètre
    
    new Node(options[param1],new_node);
    new Node(options[param2],new_node);
    
  }
  if (options[i]=="!"){
  
    int param1=rand()%5;  //choix d'un paramètre
    
    new Node(options[param1],new_node);
  }
  
  if (options[i]=="true"){
    new_node->setboolvalues(true);
  }
  if (options[i]=="false"){
    new_node->setboolvalues(false);
  }
  
  
  if (father!=nullptr and father->NextNode1()==node){
    father->setNextNode(new_node);
  }
  if (father!=nullptr and father->NextNode2()==node){
    father->setNextNode2(new_node);
  }
  new_node->setFatherNode(father);
}
 

//getter
Node * Tree:: FirstNode(){
  return FirstNode_;
}


int Tree::Fitness(){
  return Fitness_;
}


int Tree::NbrNode(){
  cross(FirstNode_);
  return NbrNode_;
}

void Tree::cross(Node * node){ 
  if (node ->values()=="&&"){
    NbrNode_=NbrNode_+1;
    cross(node -> NextNode1());
    cross(node -> NextNode2());
  }
  if (node -> values()=="||"){
    NbrNode_=NbrNode_+1;
    cross(node -> NextNode1());
    cross(node -> NextNode2());
  }
  if (node -> values()=="!"){
    NbrNode_=NbrNode_+1;
    cross(node -> NextNode1());
  }
  else{
    NbrNode_=NbrNode_+1;
  }
}



//setter
Tree * Tree::nextTree(){
  return(nextTree_);
}


void Tree::generation(int x){
  generation_=x;
}







void Tree::calcul_fitness(Matrix matrix){

  int nligne=matrix.nlignes();
  int ncolonne=matrix.ncolonnes();
  bool ** x_=matrix.x();
  bool * y_=matrix.y(); 

  for (int i=0;i<nligne;++i){//Calcul of fitness
    bool x[ncolonne];
    x[0]=x_[i][0];
    x[1]=x_[i][1];
    x[2]=x_[i][2];
    bool y=y_[i];
    if (cross(FirstNode_,x)!=y){
      Fitness_-=1;
    }
  } 
}


bool Tree::cross(Node * node,bool * x){ // WARNING &nb_node_son permet de compter le nombre de noeud à partir du node passé en paramètre, ce n'est pas le nombre d'élément de l'arbre en entier!!! Il est passé en adresse et oblige de l'initialiser à 0 à chaque utilisation de cross//
  if (node ->values()=="&&"){
    return (cross(node -> NextNode1(),x) and cross(node -> NextNode2(),x));
  }
  if (node -> values()=="||"){
    return (cross(node -> NextNode1(),x) or cross(node -> NextNode2(),x));
  }
  if (node -> values()=="!"){
    return (not cross(node -> NextNode1(),x));
  }
  if (node -> values()=="x1"){
    return (x[0]);
  }
  if (node -> values()=="x2"){
    return (x[1]);
  }
  if (node -> values()=="x3"){
    return (x[2]);
  }
  else{
    return (node->bool_values());
  }
}







std::string Tree::show(){
  return(parcour(FirstNode_));
}


std::string Tree::parcour(Node * node){
  if (node -> values()=="||" or node -> values()=="&&"){
    return("(" + parcour(node->NextNode1()) + node->values() + parcour(node->NextNode2()) + ")");
  }
  if (node -> values()=="!"){
    return(node->values() + parcour(node->NextNode1()));
  }
  if (node -> type()=="bool"){
    return(node->bool_values() ? "true" : "false");
  }
  else{
    return(node->values());
  }
}



void Tree::link(Tree * tree){
  nextTree_=tree;
}

void Tree::sup(){
  FirstNode_->sup();
  delete this;
}


