# **README**

## **Approach**

### **Task 1: Polynomial Using Singly Linked List**
- Used **singly linked list** to store terms of polynomial as nodes.  
- For insertion:
  - If exponent is greater than head’s exponent → insert before head.  
  - If exponent is smaller than tail’s exponent → insert after last node.  
  - Otherwise, insert in correct position using `loc` and `ploc`.  
- For addition: added coefficients of nodes with the same exponent.  
- For multiplication: multiplied each term of one polynomial with all terms of the other.  
- For derivative: multiplied coefficient with exponent and reduced exponent by 1.  

---

### **Task 2: Text Editor Using Doubly Linked List**
- Used **doubly linked list** to store characters and cursor.  
- Initially, list has one node containing the **cursor ('|')**.  
- For insertion: found node where `char == '|'` and inserted new node before it.  
- For deletion: found cursor node and deleted the node before it.  
- For moving cursor:
  - Located cursor node (`'|'`).  
  - Moved it left or right by swapping it with its previous or next node.  
  - Used doubly linked list for easy access to both directions.  

---

### **Task 3: UNO Game Simulation**
- Used **OOP** and **DSA** concepts to design the UNO game using **classes and enums**.  
- Used **vector** for deck (easy shuffle) and **stack** for discard pile (access top card only).  
- Each player stores cards in a **vector** (their hand).  
- Built and shuffled deck using **mt19937 random engine**.  
- Dealt 7 cards to each player and placed one card on discard pile to start the game.  
- Each turn:
  - Player tries to play a matching card (same color or value).  
  - If no match, player draws one card and plays it if possible.  
- **Action cards** (Skip, Reverse, Draw Two) modify game flow.  
- **Direction** managed using enum (Clockwise / Counter-clockwise).  
- Game continues until one player has no cards → declared **winner**.  

---

## **Challenges Faced**
- The main challenge was using the given header file **without modifying it**.  
- To solve this, created a **helper class (`unoImpl`)** with extra attributes (`start`, `loc`, `ploc`, etc.).  
- The helper class handled core logic, and the main class functions (defined using **scope resolution operator**) called the corresponding helper class methods.  

---


