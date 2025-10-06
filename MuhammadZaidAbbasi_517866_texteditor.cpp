#include<iostream>
#include<string>
#include "texteditor.h"

using namespace std;


class text  {
private:
	class node{
	public:
		char ch;
		node* next;
		node* prev;
		
	};
	node* start;
public:
	
	text(){
		node* cursor=new node();
		cursor->ch='|';
		cursor->next=NULL;
		cursor->prev=NULL;
		start=cursor;
	}
	
	// Insert character at cursor
    void insertChar(char c) {
		node* newNode=new node();
		newNode->ch=c;
		newNode->next=NULL;
		newNode->prev=NULL;

		node* temp=start;
		while(temp &&temp->ch!='|'){
			temp=temp->next;
		}
		node* before=temp?temp->prev:NULL;
		
		newNode->next=temp;
		newNode->prev=before;
		
		if(temp) 
			temp->prev=newNode;
	    if (before)
            before->next = newNode;
        else
            start = newNode;
		cout<<"After Insert \'"<<c<<"\': "<<getTextWithCursor()<<endl;

	}

    // Delete character before cursor
    void deleteChar() {
		node* cursor=start;
		while(cursor && cursor->ch!='|'){
			cursor=cursor->next;
		}
		if(!cursor || cursor==start) return;
		
		node* toDel=cursor->prev;
		if(toDel==start){
			start=cursor;
			cursor->prev=NULL;
			delete toDel;
		}
		else{
			node* before=toDel->prev;
			before->next=cursor;
			cursor->prev=before;
			delete toDel;
		}
		cout<<"After delete:     "<<getTextWithCursor()<<endl;

	}

    // Move cursor one position left
    void moveLeft() {
		
	    node* cursor = start;
	    while (cursor && cursor->ch != '|') 
	        cursor = cursor->next;
	    
	    if(cursor==start) return;
	    
	    node* left = cursor->prev;
	    node* beforeLeft = left->prev;
	    node* afterCursor = cursor->next;
	    
	    if(beforeLeft) beforeLeft->next=cursor;
	    else start=cursor;
	    
	    cursor->prev = beforeLeft;
		cursor->next=left;    
	    left->prev = cursor;
    	left->next = afterCursor;
    	
    	if (afterCursor) afterCursor->prev = left;
		cout<<"After Move Left:  "<<getTextWithCursor()<<endl;

	}

    // Move cursor one position right
    void moveRight() {
		node* cursor = start;
	    while (cursor && cursor->ch != '|') {
	        cursor = cursor->next;
	    }
	    if(!cursor->next) return;
	    
	    node* right = cursor->next;
        node* afterRight = right->next;
        node* beforeCursor = cursor->prev;
        
        if (beforeCursor) beforeCursor->next = right;
        else start = right;
        
        right->prev = beforeCursor;
		right->next = cursor;
		cursor->prev = right;
		cursor->next = afterRight;
		
		if (afterRight) afterRight->prev = cursor;
		cout<<"After Move Right: "<<getTextWithCursor()<<endl;

	}

    // Return string with cursor position
    std::string getTextWithCursor() const {
		string result="";
		node* temp=start;
		while(temp){
			result+=temp->ch;
			temp=temp->next;
		}
	return result;
	}
    
};
static text *impl= new text();
	
    void TextEditor::insertChar(char c)  { impl->insertChar(c); }
	void TextEditor::deleteChar()  { impl->deleteChar(); }
    void TextEditor::moveLeft()  { impl->moveLeft(); }
    void TextEditor::moveRight()  { impl->moveRight(); }
    string TextEditor::getTextWithCursor() const  { return impl->getTextWithCursor(); }


  

int main(){
TextEditor ed;

ed.insertChar('a');
ed.insertChar('b');
ed.moveLeft();
ed.insertChar('c');
ed.deleteChar();
ed.moveLeft();
ed.moveLeft();
ed.moveRight();
ed.moveRight();
ed.deleteChar();
ed.deleteChar();


return 0;
}
