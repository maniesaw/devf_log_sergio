#include "tree.h"
#include "forest.h"
#include "matrix.h"


Forest::Forest() {
  this -> nb_elmts_ = 0; 
  this -> firstTree_ = nullptr;
  this -> lastTree_= nullptr;
  this -> lambda_= 5;
}

Forest::Forest(int lambda) {
  this -> nb_elmts_ = 0; 
  this -> firstTree_ = nullptr;
  this -> lastTree_= nullptr;
  this -> lambda_= lambda;
}



void Forest::PushBack(Tree * tree){
  if (firstTree_!=nullptr){
    lastTree_->link(tree);
  }
  if (firstTree_==nullptr){
    firstTree_=tree;
  }
  lastTree_=tree;
  tree->generation(nb_elmts_);
  nb_elmts_+=1;
} 



void Forest::show(){
  Tree * tmp=firstTree_;
  for (int i=0;i<=nb_elmts_-1;++i){
    std::cout<< "génération "  << i << " : " << tmp->show() <<std::endl;
    tmp = tmp->nextTree();
  }
}

Tree * Forest::show(int i){
  Tree * tmp=firstTree_;
  int k=0;
  while (k != i){
    tmp = tmp->nextTree();
    ++k;
  }
  return(tmp);
}


int Forest::nb_elmts(){
	return nb_elmts_;
}

Tree * Forest::lastTree(){
  return(lastTree_);
}

Tree * Forest::firstTree(){
  return(firstTree_);
}


void Forest::evolve(Matrix matrix_){

  bool ** xt_=matrix_.x();
  bool * yt_=matrix_.y(); 

  int nligne=matrix_.nlignes();
  int ncolonne=matrix_.ncolonnes();


  Tree * fiel[lambda_+1]; //Stock children and one parent
  Tree * best = this->lastTree();


       // génération (after while+condition)
  Tree * parent=lastTree_; 
  fiel[0]=parent;
  best=parent;
	for (int k=1;k<lambda_+1;++k){   // potentiels enfants
		//mutation
		fiel[k]= new Tree(parent);
		fiel[k]->mutation2();
		
    std::cout<<fiel[k]->show()<<std::endl;

		for (int i=0;i<nligne;++i){//Calcul of fitness
		  bool xt[ncolonne];
		  xt[0]=xt_[i][0];
		  xt[1]=xt_[i][1];
		  bool yt=yt_[i];
		  fiel[k]->calcul_fitness(xt,yt);
		}
	  std::cout<< "fitness : " << fiel[k]->Fitness() <<std::endl;
	  if (best->Fitness()<fiel[k]->Fitness()){
	    best=fiel[k];
  	}
  }
	
	std::cout<<std::endl;
  std::cout<< "Best : " << best->show() <<std::endl;
  std::cout<<std::endl;
  
	PushBack(best);
}

void Forest::generation(int nb_gen,Matrix matrix_){
  for (int g=0;g<nb_gen;++g){  
    evolve(matrix_);
  }
}
