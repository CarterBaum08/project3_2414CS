#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <class DT>
class NestedBST {
  public:
    DT value; // value of the node
    vector<int> keys; // vector of keys associated with this value
    int dimension; // dimension of this node
    NestedBST* left; // pointer to left child
    NestedBST* right; // pointer to right child
    NestedBST* innerTree; // pointer to nested BST (next dimension)
    // ----- Constructors -----
    NestedBST(); // default constructor
    NestedBST(DT val, int dim); // parameterized constructor
    // ----- Core Operations -----
    void insert(int key, const vector<DT>& values); // insert or update a key-value tuple
    void find(const vector<DT>& pattern); // find keys matching a pattern with wildcards
    void display(int indent = 0); // print tree structure for verification
    //--------------------- stuff I added as helper
    bool isEmpty; //added as a helper
    void findHelper(const vector<DT>& pattern, bool& found);//ai suggestion to help track if a match was found
}; 

//--all of these "BST" are just value objects in a specified dimension that have candiate key(s) 
//--associated with them and three pointers: left, right, and innerTree.
template <class DT> 
NestedBST<DT>::NestedBST(){ //this is an empty BST object with no value, dim, or keys set
  left = NULL; // no left tree 
  right= NULL; // no right tree
  innerTree = NULL; // no inner tree
  dimension = 0; // dimension 0 exists because we created the node
  //no set value ,dim, or keys 
  isEmpty = true;
};
template <class DT> 
NestedBST<DT>::NestedBST(DT val, int dim){ //this is BST object with a value, dimension but no keys set
  value = val; //--parameter of value
  dimension=dim;//--paramter of dimension, ie number of dimensions 
  left = NULL; // no left tree
  right=NULL; // no right tree
  innerTree = NULL; // no inner tree
  //no set keys 
  isEmpty=false;
};

//--The purpose of this function is to insert or update the mapping with the given values and key paired with
  //--those values. The values will either be new or already existing. if it is new, add the values and the 
  //--key associated with it(keys are stored in an vector for every value). if it already exists, compare the
  //--keys and either replace said key or do nothing if it is the same.
  //--size of the vector values is the number of dimensions
template <class DT> 
void NestedBST<DT>::insert(int key, const vector<DT>& values)//key and value tuple 
{
  if(dimension >= values.size()) return; //protects from going out of bounds
  DT currentVal = values[dimension];//tracks the current value


  if(isEmpty)//if the bst is empty, VERY FIRST insert 
  {
    value = currentVal; //setting the BST object's value,
    keys.push_back(key);//adding the key to this object's candiates list(keys vector)
    if(dimension == values.size()-1)cout<<"Inserted key=";//edge case
    isEmpty = false;
    if(dimension < values.size()-1) //checks to make sure that we do not index out of bounds
    {
      innerTree = new NestedBST<DT>();//dim = 0 and value not set
      innerTree->dimension = dimension+1;
      innerTree->insert(key,values);//recursive part
    }
    return;
  }

  //ALL OTHER INSERTS 
  //First check to see if we need to add a value to the dimension 
  if(currentVal< value)//go left
  {
    if(left == NULL)//only node on left
    {
      left = new NestedBST<DT>(currentVal,dimension);//create a new dim& value  node
    }
    left->insert(key,values); //recurse to find the value
  }
  else if(currentVal > value)//go right 
  {
    if(right == NULL)//the only node on right
    {
    right = new NestedBST<DT>(currentVal,dimension);//dim& value set
    }
    right->insert(key,values);
  }

  else{// we are on the matching value and do not need to create a new node 
    if(dimension < values.size()-1)//if not the last value 
    {
      bool hasKey = false;//does the value have the key yet?
      for(int i=0; i< keys.size();i++)
      {
        if(key == keys[i]) hasKey = true;//has key
      }
      if(!hasKey)
      {
        keys.push_back(key);//adding the key to this object's candiates list(keys vector)
      }
      if(innerTree == NULL)
      {
        innerTree = new NestedBST<DT>(values[dimension+1],dimension+1);
      }
      innerTree->insert(key,values);//after it has a key then we can descend 
    }
    else{//last value
      if(keys.empty())
      {
        keys.push_back(key);
        cout<<"Inserted key=";
      }
      else if(keys[0] != key){
        keys[0] = key;
        cout<<"Updated key=";
      }
      else{
        cout<<"Unchanged key=";
      }
        
    }
  }
  
};
//--We are given a pattern of values that are either the value data type or a *. * means it can be any value
  //--in its dimension that fits the pattern(wildcard). Do BST comparison by going left or right and then when
  //--when you find the first value at dimension descend to the nextTree(innerTree) and do it again. if the
  //--value is a *, you have to traverse both left and right. When you reach the end, collect the single key.
  //--Match will either be found or not. Because it is a void method, It prints out the key for a specifed 
  //--pattern like so: "key=123 for (10,23,5)" or "EMPTY"
template<class DT>//FINISHED
void NestedBST<DT>::find(const vector<DT>& pattern) {
    bool found = false;
    findHelper(pattern, found);  // call recursive helper
    if(!found)
    {
      cout << "EMPTY" << endl;
    }
}
template <class DT> 
void NestedBST<DT>::findHelper(const vector<DT>& pattern, bool& found){
  if(this == nullptr) return;

  //---AI Assisted---
  static vector<DT> path; //creating a stored path of where we go
  if (path.size() <= dimension) //if the path vector is less than the total dimensions then add 
      path.push_back(value); 
  else
      path[dimension] = value;// change the the specified dimension since the vector is already full
  //------------------

  if(pattern[dimension] == -999999)//--first checker to see if it is a wild card
  {
    if(dimension<pattern.size()-1)//if it is not the last pattern card, checking dimension
    {
      left->findHelper(pattern,found);//considers left possibilty
      innerTree->findHelper(pattern,found); //--descend to next dimension 
      right->findHelper(pattern,found);//considers right possibilty
      
    }
    else if(dimension == pattern.size() - 1)//last pattern card is a *
    {
      //--final value, not sure what to print, this is also where the key value is located
      //-AI assisted----------
      cout << "key=" << keys[0] << " for (";
      for (int j = 0; j < path.size(); j++) {
          cout << path[j];
          if (j< path.size() - 1) 
          {
            cout << ",";
          }
      }
        cout << ")" << endl;
      //------------This outputs the path for a specific key
      found = true;
      left->findHelper(pattern,found);
      right->findHelper(pattern,found);
    }

  }
  else{//it is not a wild card 
    if(pattern[dimension] < value ){//--if the card is less than the current value
        left->findHelper(pattern,found);
      }
    else if(pattern[dimension] > value )
    {
      right->findHelper(pattern,found);
    }
    else{//we're on the correct value
      if(dimension<pattern.size()-1)//if it is not the last pattern card, checking dimension
      {
        if(value == pattern[dimension])//--if the card we are on is the same 
        { 
          innerTree->findHelper(pattern,found); //--descend to next dimension 
        }
      }
      else//last pattern card
      {
        //-AI assisted----------
        cout << "key=" << keys[0] << " for (";
        for (int j = 0; j < path.size(); j++) {
            cout << path[j];
            if (j< path.size() - 1) 
            {
              cout << ",";
            }
        }
        cout << ")" << endl;
        found = true;
        //-----------This outputs the path for a specific key
      }
      
    }
  }
}


template <class DT> 
void NestedBST<DT>::display(int indent){//FINISHED
  //--The display method is inorder at each dimension, it displays the dimension, then value, then how many
  //--candiates(keys) it has. Then it descends a dimension and repeats until there only exists one key in the
  //--value dimension pair, where instead of printing the candiates number, it prints the actual singular key.
  
  //--Steps to traverse in order--
  if(this == nullptr)return;//1.If the root is NULL, return and display nothing.
  left->display(indent);//2.Recursively display the left subtree:--call display(root.nextTree, dimension, indent)
  //3.Print the current nodes information:
  for(int i=0; i<indent;i++)cout<<" ";//--indent output by indent spaces
  cout<< "[dim " <<dimension<< "]"<<" value=" <<value<<" ";//--print[Dim dimension]value = <root.value>
  if(keys.size()==1 && innerTree==NULL)//--If this node's dimension is the last one, print the key
  {
    cout<<" key="<<keys[0]<<endl;
  }
  else//--else print how many candidate keys are velow this node
  {
    cout<<" (candidates="<<keys.size()<<")"<<endl;
  }
  
  if(innerTree!=NULL)//4.If a nested tree exists(root.innerTree is not NULL):
  {
    int nextIndent=indent +2;//--increase indention by two spaces
    for(int i=0; i<nextIndent;i++)cout<< " ";//--print a marker line: indent spaces+ "-> dim(dimension+1)"
    cout<<"-> dim "<<dimension+1<<endl;
    innerTree->display(nextIndent+2);//--recursively display the nested tree: Display(root.nextTree, dimension+1, indent+2)
  }
  right->display(indent);//5.Recursively display the right subtree:Display(root.right, dimension, indent)
}

int main() {


  int numDimensions; // number of value dimensions
  cin >> numDimensions;

  // Create the root tree (dimension 0)
  NestedBST<int>* root = new NestedBST<int>();
  root->dimension = 0;

  int numCommands;
  cin >> numCommands;

  char command;
  for (int i = 0; i < numCommands; i++) {
    cin >> command;
    switch (command) {
      case 'I': { // Insert
        int key;
        cin >> key;
        vector<int> values(numDimensions);
        for (int d = 0; d < numDimensions; d++) {
          cin >> values[d];
        }
        root->insert(key, values);
        /*
        "Inserted key=" <<
        */
        cout << key << " for (";
        for (int d = 0; d < numDimensions; d++) {
          cout << values[d];
          if (d < numDimensions - 1) cout << ",";
        }
        cout << ")" << endl;
        break;
  }
  case 'F': { // Find
    vector<int> pattern(numDimensions);
    string token;
    for (int d = 0; d < numDimensions; d++) {
      cin >> token;
      if (token == "*") {
        // Represent wildcard as sentinel (e.g., -999999)
        pattern[d] = -999999;
    } else {
      pattern[d] = stoi(token);
    }
    }
    /*
    cout << "Find pattern: (";
    for (int d = 0; d < numDimensions; d++) {
      if (pattern[d] == -999999) cout << "*";
      else cout << pattern[d];
      if (d < numDimensions - 1) cout << ",";
    }
    cout << ")" << endl;
    */
    root->find(pattern);
    break;
  }
  case 'D': { // Display
    cout << "NestedBST Structure:" << endl;
    root->display();
    cout<<endl;
    break;
  }
  default:
    cout << "Unknown command: " << command << endl;
    break;
  }
  }
  // Clean up
  delete root;
  return 0;
}