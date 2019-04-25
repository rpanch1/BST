# BST
Data Structure - Project 3

For this project, I had to modify a binary search tree to implement size balancing along with some other new features.

Additional features added

- to_vector: 
  creates a vector and populates it with the     elements of the tree (in-order) and returns the vector     as a pointer. runtime: linear.
  
- get_ith: 
  determines the ith smallest element in t and      "passes it back" to the caller via the reference         parameter x.  i ranges from 1..n where n is the number of elements in the tree. runtime: height of tree.
  
- position_of: 
  similar to the reverse of get_ith. runtime: height of tree.
  
- num_geq: 
  returns the number of elements in tree which are greater than or equal to x. runtime: height of tree.
  
- num_leq: 
  returns the number of elements in tree which are less han or equal to x. runtime: height of tree.
  
- num_range:
  returns the number of elements in tree which are between min and max (inclusive). runtime: height of        tree.
  
- extract_range: 
  allocates a vector of element type T and populates it with the tree elements between min and max in       order. A pointer ot the allocated and populated is returned. runtime: height of tree + k num of elements    in the range.
  
Size-balancing property formula: 
  
  max(num_left_nodes, num_right_nodes) <= 2 * min(num_left_nodes, num_right_nodes) + 1
