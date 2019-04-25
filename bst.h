#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>

template <typename T>
class bst {

private: 
  struct bst_node {
    T      val;
    bst_node *left;
    bst_node *right;
    int left_nodes;
    int right_nodes;

    bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr, int leftn = 0, int rightn = 0)
      : val { _val },  left { l }, right {r}, left_nodes{leftn}, right_nodes{rightn}
    { }
  };

public:
  // constructor:  initializes an empty tree
  bst(){
    root = nullptr;
  }

private:
  // helper function which recursively deallocates nodes
  //   in a tree.
  static void delete_nodes(bst_node *r){
    if(r==nullptr) return;
    delete_nodes(r->left);
    delete_nodes(r->right);
    delete r;
  }

public:
  // destructor
  ~bst() {
    delete_nodes(root);
  }

private:

  /**
   * function:  insert()
   * desc:      recursive helper function inserting x into
   *            binary search tree rooted  at r.
   *
   * returns:   pointer to root of tree after insertion.
   *
   * notes:     if x is already in tree, no modifications are made.
   */
  static bst_node * _insert(bst_node *r, T & x, bool &success){
    if(r == nullptr){
      success = true;
      return new bst_node(x, nullptr, nullptr);
    }
    if(r->val == x){
      success = false;
      return r;
    }
    if(x < r->val){
      r->left = _insert(r->left, x, success);
      if(success) r->left_nodes++;
    }
    else {
      r->right = _insert(r->right, x, success);
      if(success) r->right_nodes++;
    }
    if(!is_sizeBalanced(r)) {
      return size_balanced(r);
    }
    return r;
  }


public:
  /**
   * function:  insert
   * desc:      inserts x into BST given by t.  Note that
   *            a BST stores a SET -- no duplicates.  Thus,
   *            if x is already in t when call made, no 
   *            modifications to tree result.
   *
   * note:      helper function does most of the work.
   *
   */
  bool insert(T & x){
    bool success;
    root = _insert(root, x, success);
    return success;
  }

  /**
   * function:  contains()
   * desc:      returns true or false depending on whether x is an
   *            element of BST (calling object)
   *
   */
  bool contains(const T & x){
    bst_node *p = root;

    while(p != nullptr){

      if(p->val == x)
        return true;
      if(x < p->val){
        p = p->left;
      }
      else
        p = p->right;
    }
    return false;  
  }

private:
  // returns pointer to node containing
  //   smallest value in tree rooted at r
  static bst_node * _min_node(bst_node *r ){
    if(r==nullptr)
      return nullptr; // should never happen!
    while(r->left != nullptr)
      r = r->left;
    return r;
  }

  // returns pointer to node containing
  //   largest value in tree rooted at r
  static bst_node * _max_node(bst_node *r ){
    if(r==nullptr)
      return nullptr; // should never happen!
    while(r->right != nullptr)
      r = r->right;
    return r;
  }

  // recursive helper function for node removal
  //   returns root of resulting tree after removal.
  static bst_node * _remove(bst_node *r, T & x, bool &success){
    bst_node *tmp;
    bool sanity;

    if(r==nullptr){
      success = false;
      return nullptr;
    }
    if(r->val == x){
      success = true;

      if(r->left == nullptr){
        tmp = r->right;
        delete r;
        return tmp;
      }
      if(r->right == nullptr){
        tmp = r->left;
        delete r;
        return tmp;
      }
      // if we get here, r has two children
      r->val = _min_node(r->right)->val;
      r->right = _remove(r->right, r->val, sanity);
      if(success) r->right_nodes--;
      if(!sanity)
        std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
      return r;
    }
    if(x < r->val){
      r->left = _remove(r->left, x, success);
      if(success) r->left_nodes--;
    }
    else {
      r->right = _remove(r->right, x, success);
      if(success) r->right_nodes--;
    }
    return r;
  }

public:

  bool remove(T & x){
    bool success;
    root = _remove(root, x, success);
    return success;
  }

private:
  // recursive helper function to compute size of
  //   tree rooted at r
  static int _size(bst_node *r){
    if(r==nullptr) return 0;
    return _size(r->left) + _size(r->right) + 1;
  }

  public: 
  int size() {
    return _size(root);
  }

private:

  static int _height(bst_node *r){
    int l_h, r_h;

    if(r==nullptr) return -1;
    l_h = _height(r->left);
    r_h = _height(r->right);
    return 1 + (l_h > r_h ? l_h : r_h);
  }

public:

  int height() {
    return _height(root);
  }

  bool min(T & answer) {
    if(root == nullptr){
      return false;
    }
    answer = _min_node(root)->val;
    return true;
  }

  T max() {
    return _max_node(root)->val;
  }

  /******************************************
   *
   * "stubs" for assigned TODO functions below 
   *
   *****************************************/
  
  /* TODO //? DONE
    * Function:  to_vector
    * Description:  allocates a vector of type T and populates
    *               it with the elements of the tree in sorted
    *               order.  A pointer to the vector is returned.
    *
    * runtime:  O(n) where n is the tree size.
    *
    */
  std::vector<T> * to_vector() {
    std::vector<T> *sorted_vec = new std::vector<T>;
    if(root == nullptr){
      return sorted_vec;
    }else{
      sorted_vec = to_vectors_helper(sorted_vec, root);
    }
    return sorted_vec;
  }


  /* TODO //?DONE
    * Function:  get_ith
    * Description:  determines the ith smallest element in t and
    *    "passes it back" to the caller via the reference parameter x.  
    *    i ranges from 1..n where n is the number of elements in the 
    *    tree.
    *
    *    If i is outside this range, false is returned.
    *    Otherwise, true is returned (indicating "success").
    *
    * Runtime:  O(h) where h is the tree height
    */
  bool get_ith(int i, T &x) {
    if(i < 1 || root == nullptr || root->left_nodes + root->right_nodes + 1 < i){
      return false;
    }
    else{
      x = get_ith_helper(i, root);
      return true;
    }
  }


  /* 
    * get_ith_SLOW:  slow version of get_ith which gives the right answer, but
    *   does so by "walking the entire tree".
    * might be helpful for debugging?
    * Private helper below does most of the work
    */
  bool get_ith_SLOW(int i, T &x) {
    int n = size();
    int sofar=0;

    if(i < 1 || i > n) 
      return false;

    x = _get_ith_SLOW(root, i, x, sofar);
    return true;
  }

private:

  //recursive helper function for get_ith()
  static T get_ith_helper(int i, bst_node *r){
    
    if(r->left_nodes + 1 == i){
      return r->val;
    }
    else if (r->left_nodes >= i){
      get_ith_helper(i,r->left);
    }
    else{
      i = i-r->left_nodes-1;
      get_ith_helper(i,r->right);
    }
  }

  //recursive helper function for to_vector
  static std::vector<T> * to_vectors_helper(std::vector<T> *v, bst_node *r){

    if(r->left != nullptr){
      v = to_vectors_helper(v, r->left);
    }
    v->push_back(r->val);
    if(r->right != nullptr){
      v = to_vectors_helper(v, r->right);
    }
    return v;
  }
  
  // recursive helper function that does most of the work
  static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
    if(t==nullptr) 
      return;
    _get_ith_SLOW(t->left, i, x, sofar);

    if(sofar==i) 
      return;
    sofar++;
    if(sofar==i) {
      x = t->val;
      return;
    }
    _get_ith_SLOW(t->right, i, x, sofar);
  }

public:

/* TODO //? DONE
  * Function: position_of 
  * Description:  this is like the inverse of
  *       get_ith:  given a value x, determine the 
  *       position ("i") where x would appear in a
  *       sorted list of the elements and return
  *       the position as an integer.
  *       If x is not in the tree, -1 is returned.
  *       Examples:
  *           if x happens to be the minimum, 1 is returned
  *           if x happens to be the maximum, n is returned where
  *               n is the tree size.                  
  *                  
  *       Notice the following property:                    
  *           Now consider the following:
  *       
  *       
  * Return:  -1 if x is not in the tree; otherwise, returns the position where x 
  *          would appear in the sorted sequence of the elements of the tree (a
  *          value in {1..n}          
  *
  * Runtime:  O(h) where h is the tree height
  */
int position_of(const T & x) {
  int i = 0;
  position_of_helper(x, root, i);
  return i;
}

/* TODO //? DONE
  * Function:  num_geq
  * Description:  returns the number of elements in tree which are 
  *       greater than or equal to x.
  *
  * Runtime:  O(h) where h is the tree height
  */
int num_geq(const T & x) {
  int i = 0;
  num_geq_helper(x, root, i);
  return i;  
}

/*
  * function:     num_geq_SLOW
  * description:  same functionality as num_geq but sloooow (linear time)
  *                (private helper does most of the work)
  *
  */
int num_geq_SLOW(const T & x) {
  return _num_geq_SLOW(root, x);
}

private:
  static void num_geq_helper(const T & x, bst_node *r, int &i){
    //check if root node is null
    if(r == nullptr);

    //if x is less then or equal to node val, go to left subtree
    else if(x <= r->val){
      i += r->right_nodes+1;
      num_geq_helper(x, r->left, i);
    }
    // if x is greater then node val go to right subtree
    else {
      num_geq_helper(x, r->right, i);
    }
  }
  static void position_of_helper(const T & x, bst_node *r, int &i){
    //check if root node is null
    if(r == nullptr){
      i = -1;
      return;
    }
    //check if x is the curent node
    else if(r->val == x){
      i += r->left_nodes+1;
      return;
    }
    //if x is less then current node go to left subtree
    else if(x < r->val){
      position_of_helper(x, r->left, i);
      return;
    }
    //if x is more then current node go to right subtree
    else{
      i += r->left_nodes+1;
      position_of_helper(x, r->right, i);
      return;
    }
  }


  static int _num_geq_SLOW(bst_node * t, const T & x) {
    int total;

    if(t==nullptr) return 0;
    total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);

    if(t->val >= x)
      total++;
    return total;
  }

public:

  /* TODO //?DONE
    * Function:  num_leq
    * Description:  returns the number of elements in tree which are less
    *      than or equal to x.
    *
    * Runtime:  O(h) where h is the tree height
    *
    **/
  int num_leq(const T &x) {
    int i = 0;
    num_leq_helper(x, root, i);
    return i;
  }

  /*
    * function:     num_leq_SLOW
    * description:  same functionality as num_leq but sloooow (linear time)
    *               (private helper _num_leq_SLOW does most of the work)
    */
  int num_leq_SLOW(const T & x) {
    return _num_leq_SLOW(root, x);
  }

private:
  // helper for num_leq
  static void num_leq_helper(const T & x, bst_node *r, int &i){
    //check if root node is null
    if(r == nullptr);

    //if x is greater then or equal to node val, go to right subtree
    else if(x >= r->val){
      i += r->left_nodes+1;
      num_leq_helper(x, r->right, i);
    }
    // if x is less then node val go to right subtree
    else {
      num_leq_helper(x, r->left, i);
    }
  }

  // helper for num_leq_SLOW
  static int _num_leq_SLOW(bst_node *t, const T &x) {
    int total;

    if(t==nullptr) return 0;
    total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);

    if(t->val <= x)
      total++;
    return total;
  }

public:

  /* TODO //?DONE
    * Function:  num_range
    * Description:  returns the number of elements in tree which are
    *       between min and max (inclusive).
    *
    * Runtime:  O(h) where h is the tree height
    *
    **/
  int num_range(const T & min, const T & max) {
    int i = 0;
    int j = 0;
    num_geq_helper(min, root, i);
    num_leq_helper(max, root, j);
    return (i+j)-(root->left_nodes+root->right_nodes+1);
  }

  /*
    * function:     num_range_SLOW
    * description:  same functionality as num_range but sloooow (linear time)
    *               (private helper _num_range_SLOW does most of the work)
    *
    */
  int num_range_SLOW(const T & min, const T & max) {
    return _num_range_SLOW(root, min, max);
  }

private:
  static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
    int total;

    if(t==nullptr) return 0;
    total =_num_range_SLOW(t->left, min, max) + 
              _num_range_SLOW(t->right, min, max);

    if(t->val >= min && t->val <= max)
      total++;
    return total;
  }

public:

  /* //?DONE
    * TODO
    * function:     extract_range
    * Description:  allocates a vector of element type T
    *               and populates it with the tree elements
    *               between min and max (inclusive) in order.  
    *               A pointer to the allocated and populated
    *               is returned.
    *
    * notes/comments:  even if the specified range is empty, a
    *                  vector is still allocated and returned;
    *                  that vector just happens to be empty.
    *                  (The function NEVER returns nullptr).
    *
    * runtime:  the runtime requirement is "output dependent".
    *           Let k be the number of elements in the specified range
    *           (and so the length of the resulting vector) and let h
    *           be the height of the tree.  The runtime must be:
    *
    *                  O(h + k)
    *
    *           So...while k can be as large as n, it can be as small
    *           as zero.  
    *
    */
  std::vector<T> * extract_range(const T & min, const T & max) {
    std::vector<T> *sorted_vec = new std::vector<T>;
    if (max < min) return sorted_vec;
    sorted_vec = extract_range_helper(min, max, root, sorted_vec);
    return sorted_vec;
  }

  private:
    // helper for extract_range
    static std::vector<T> * extract_range_helper(const T & min, const T & max, bst_node *r, std::vector<T> *v){
      if (r == nullptr);

      else if(min < r->val && max < r->val){
        v = extract_range_helper(min, max, r->left, v);
      }
      else if(min > r->val && max > r->val){
        v = extract_range_helper(min, max, r->right, v);
      }
      else if(min == r->val && max == r->val){
        v->push_back(r->val);
      }
      else if(min < r->val && max == r->val){
        v = extract_range_helper(min, max, r->left, v);
        v->push_back(r->val);
      }
      else if(min < r->val && max > r->val){
        v = extract_range_helper(min, max, r->left, v);
        v->push_back(r->val);
        v = extract_range_helper(min, max, r->right, v);
      }
      else{
        v->push_back(r->val);
        v = extract_range_helper(min, max, r->right, v);
      }
      return v;
    }

  /***************************************************
  *  PREORDER AND INORDER TREE TRAVERSALS BELOW      * 
  *                                                  * 
  ***************************************************/

  
  static void indent(int m){
    int i;
    for(i=0; i<m; i++)
      std::cout << "-";
  }

  static void _inorder(bst_node *r){
    if(r==nullptr) return;
    _inorder(r->left);
    std::cout << "[ " << r->val << " ]\n";
    _inorder(r->right);
  }

  static void _preorder(bst_node *r, int margin){
    if(r==nullptr) {
      indent(margin);
      std::cout << " nullptr \n";
    } 
    else {
      indent(margin);
      std::cout << "[ " << r->val << " ]\n";
      _preorder(r->left, margin+3);
      _preorder(r->right, margin+3);
    }
  }

public:
  void inorder() {
    std::cout << "\n======== BEGIN INORDER ============\n";
    _inorder(root);
    std::cout << "\n========  END INORDER  ============\n";
  }


  void preorder() {

    std::cout << "\n======== BEGIN PREORDER ============\n";
    _preorder(root, 0);
    std::cout << "\n========  END PREORDER  ============\n";

  }

  /***************************************************
  *  END PREORDER AND INORDER TREE TRAVERSALS        * 
  *                                                  * 
  ***************************************************/

private:
  //checks if a tree is size balanced
  static bool is_sizeBalanced(bst_node* r){
    int min, max;
    if(r->left_nodes >= r->right_nodes){
      max = r->left_nodes; min = r->right_nodes;
    }
    else{ max = r->right_nodes; min = r->left_nodes;}
    if(max <= 2*min + 1) return true;
    else return false;
  }

  // size_balances the tree from root r
  static bst_node * size_balanced(bst_node* r) {
    std::vector<T> *sorted_vec = new std::vector<T>;
    if(r == nullptr){
      return r;
    }else{
      sorted_vec = to_vectors_helper(sorted_vec, r);
    }
    return _from_vec(*sorted_vec, 0, sorted_vec->size()-1);
  }

  /* 
    * Recursive  helper function _from_vec, used by
    * from_sorted_arr(...). The function must return a sub-tree that is
    * perfectly balanced, given a sorted array of elements a.
    */
  static bst_node * _from_vec(const std::vector<T> &a, int low, int hi){
    int m;
    bst_node *root;

    if(hi < low) return nullptr;
    m = (low+hi)/2;
    root = new bst_node(a[m]);
    root->left  = _from_vec(a, low, m-1);
    if(root->left != nullptr) root->left_nodes++;

    root->right = _from_vec(a, m+1, hi);
    if(root->right != nullptr) root->right_nodes++;
    
    return root;

  }

public:
  static bst * from_sorted_vec(const std::vector<T> &a, int n){

    bst * t = new bst();
    t->root = _from_vec(a, 0, n-1);
    return t;
  }

private:

  /*
    * DO NOT MODIFY THE TWO FUNCTIONS BELOW!!!
    */
  static void _to_preorder_vec(bst_node *t, std::vector<T> &vec) {
    if(t==nullptr) return;
    vec.push_back(t->val);
    _to_preorder_vec(t->left, vec);
    _to_preorder_vec(t->right, vec);
  }
  std::vector<T> * to_preorder_vec() {
    std::vector<T> *vec = new std::vector<T>();
    _to_preorder_vec(root, *vec);
    return vec;
  }

  /*
  * Finally, the data members of the bst class
  */
private:
  bst_node *root;


}; // end class bst

#endif
