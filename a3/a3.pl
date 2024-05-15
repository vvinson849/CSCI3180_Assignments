/* 
* CSCI3180 Principles of Programming Languages
*
* --- Declaration ---
* I declare that the assignment here submitted is original except for source
* materials explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/
*
*
* Source material acknowledgements (if any):
* 
* Students whom I have discussed with (if any):
*
*/

% 1a
is_palindrome([]).
is_palindrome([_]).
is_palindrome([Head|Tail]) :- 
    append(Middle, [Head], Tail), 
    is_palindrome(Middle).

% 1b
interleave([], [], []).
interleave([H1|T1], [H2|T2], [H1, H2| T3]) :- interleave(T1, T2, T3).

% 1c
duplicate_elements([], []).
duplicate_elements([H1|T1], [H1, H1| T2]) :- duplicate_elements(T1, T2).

% 2a
knowledge_tree(kt(m, 
                  [kt(a, 
                      [kt(r, []), kt(s, [])]), 
                  kt(g, []), 
                  kt(i, []), 
                  kt(c, 
                     [kt(h, [])])
                  ])
               ).

% 2b
node_depth(kt(Node, _), Node, s(0)).
node_depth(kt(X, Subtrees), Node, s(Depth)) :- 
    X \= Node,
    member(Subtree, Subtrees),
    node_depth(Subtree, Node, Depth).

% 2c
sum(0, X, X).
sum(s(X), Y, s(Z)) :- sum(X, Y, Z).

num_node_in_forest([], 0).
num_node_in_forest([H|T], Totalnum) :- 
    num_node(H, Hnum), 
    num_node_in_forest(T, Tnum),
    sum(Hnum, Tnum, Totalnum).

num_node(kt(_, []), s(0)).
num_node(kt(_, Forest), s(N)) :- num_node_in_forest(Forest, N).

% 2d
num_leaf_in_forest([], 0).
num_leaf_in_forest([H|T], Totalnum) :- 
    num_leaf(H, Hnum), 
    num_leaf_in_forest(T, Tnum),
    sum(Hnum, Tnum, Totalnum).

num_leaf(kt(_, []), s(0)).
num_leaf(kt(_, Forest), N) :- num_leaf_in_forest(Forest, N).

% 2e
path_to_node(kt(Dest, _), Dest, [Dest]).
path_to_node(kt(Node, Subtrees), Dest, [Node|Path]) :- 
    Node \= Dest, 
    member(Subtree, Subtrees),
    path_to_node(Subtree, Dest, Path).
