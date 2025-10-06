#include<iostream>
#include "polynomial.h"
#include<string>
#include<map>


using namespace std;
class node{
public:
    int coff;
	unsigned int exp;
	node* next;	
}; 

class  Poly {
private:
	node* start;
	node* last;
public:
	//constructor
	Poly(){
	start=NULL;
	last=NULL;
	
	} 
	    // Copy constructor (to avoid shared pointers)
    Poly(const Poly& other) {
        start = NULL;
        last = NULL;
        node* temp = other.start;
        while (temp) {
            insertTerm(temp->coff, temp->exp);
            temp = temp->next;
        }
    }


    // Insert a term into the polynomial
    void insertTerm(int coefficient, int exponent){
    	if (coefficient == 0) return;
		node *newNode=new node();
		newNode->coff=coefficient;
		newNode->exp=exponent;
		newNode->next=NULL;
		//insert at head
		if(start==NULL){ 
			start=newNode;
			last=newNode;	
		}
		//insert before head
		else if(exponent>start->exp){
			newNode->next=start;
			start=newNode;	
		}
		//insert at end
		else if(exponent<last->exp){
			last->next=newNode;
			last=newNode;	
		}
		//insert in middle   
		else{
				node* loc=start;
				node* ploc=NULL;
			while(loc && loc->exp>exponent){
					ploc=loc;
					loc=loc->next;
			}
			/*if exponent same then add coefficient and 
			dont make another node*/
			if(loc && loc->exp==exponent){
				loc->coff+=coefficient;
				delete newNode;
			}
			else{
				newNode->next=loc;
				ploc->next=newNode;
			}
		}

	}

    // Return polynomial as a human-readable string
    std::string toString() const {
    	 bool firstTerm = true;
    	if(start==NULL) return "No Terms in Polynomial";
    	
		string rslt="";
		node* temp=start;
		while(temp!=NULL){
			if(temp->coff>0 && !firstTerm )  rslt+="+";
			if(temp->exp==0)	rslt+=to_string(temp->coff);
			else if(temp->coff==0){
				temp=temp->next;
				continue;
				}
			else if(temp->exp==1){
				if(temp->coff==1) rslt+="x";
				else if(temp->coff==-1) rslt+="-x";
				else rslt+=to_string(temp->coff)+"x";
			}
			else{
				if(temp->coff==1) rslt+="x^"+to_string(temp->exp);
				else if(temp->coff==-1) rslt+="-x^"+to_string(temp->exp);
				else rslt+=to_string(temp->coff)+"x^"+to_string(temp->exp);
			}
										
			temp=temp->next;
			firstTerm=false;
			}
		return rslt;
		
	}

    // Return a new polynomial that is the sum of this and other
    Poly add(const Poly& other) const{
		Poly answer;
		node* p1=this->start;
		node* p2=other.start;
		while(p1){
			answer.insertTerm(p1->coff,p1->exp);
			p1=p1->next;
		}
		while(p2){
			answer.insertTerm(p2->coff,p2->exp);
			p2=p2->next;
		}
		
		return answer;
	}

    // Return a new polynomial that is the product of this and other
    Poly multiply(const Poly& other) const{
		Poly mul;
		for(node* p1=this->start;p1;p1=p1->next){
			for(node* p2=other.start;p2;p2=p2->next){
					mul.insertTerm(p1->coff*p2->coff,p1->exp+p2->exp);
				}
			}
		
		return mul;
		}

    // Return a new polynomial that is the derivative of this polynomial
    Poly derivative() const{
		Poly der;
		node* poly=this->start;
		while(poly){
			if(poly->exp>0)
				der.insertTerm(poly->coff*poly->exp,poly->exp-1);
			poly=poly->next;
			}
			
		return der;
	
	}
    
};

// ===============================
// Static global instance
// ===============================
static Poly* impl = nullptr;
static map<const Polynomial*, Poly> polyData;

// ===============================
// Polynomial class implementation
// ===============================

static Poly* getImpl(const Polynomial* obj) {
    impl = &polyData[obj]; // always point impl to correct one
    return impl;
}

//=========================================
// Polynomial member function definitions
//=========================================
void Polynomial::insertTerm(int coefficient, int exponent) {
    getImpl(this)->insertTerm(coefficient, exponent);
}

string Polynomial::toString() const {
    return getImpl(this)->toString();
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial result;
    Poly sum = getImpl(this)->add(*getImpl(&other));
    polyData[&result] = sum;
    return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial result;
    Poly prod = getImpl(this)->multiply(*getImpl(&other));
    polyData[&result] = prod;
    return result;
}

Polynomial Polynomial::derivative() const {
    Polynomial result;
    Poly der = getImpl(this)->derivative();
    polyData[&result] = der;
    return result;
}

int main(){

Polynomial p1;
cout<<p1.toString()<<endl;
p1.insertTerm(3, 4);
p1.insertTerm(2, 2);
p1.insertTerm(-6, 4);
p1.insertTerm(2, 3);
p1.insertTerm(-1, 1);
p1.insertTerm(5, 0);
p1.insertTerm(-9, 6);
p1.insertTerm(0, 6);
Polynomial p2;
p2.insertTerm(1, 4);
p2.insertTerm(5, 6);
p2.insertTerm(4, 1);
p2.insertTerm(-1, 0);
p2.insertTerm(-3, 1);

cout<<p1.toString()<<endl;
cout<<p2.toString()<<endl<<endl;

cout<<(p1.add(p2)).toString()<<endl<<endl;

cout<<p1.toString()<<endl;
cout<<p2.toString()<<endl<<endl;

cout<<p1.derivative().toString()<<endl;
cout<<(p2.derivative()).toString()<<endl<<endl;

cout<<p1.toString()<<endl;
cout<<p2.toString()<<endl<<endl;

cout<<p1.multiply(p2).toString()<<endl<<endl;

cout<<p1.toString()<<endl;
cout<<p2.toString()<<endl<<endl;


return 0;
}
