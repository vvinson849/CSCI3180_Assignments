(* 
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
*)

(* Part 1 *)
(* 1a *)
fun reverse lst : int list = 
    if null(lst) then nil
    else reverse(tl(lst)) @ hd(lst)::nil;

(* 1b *)
fun reverse(lst) : int list = 
let fun last(lst) : int = 
        if null(tl(lst)) then hd(lst)
        else last(tl(lst))
    fun removeLast(lst) : int list = 
		if null(lst) orelse null(tl(lst)) then nil
		else hd(lst)::removeLast(tl(lst));
in
    if null(lst) orelse null(tl(lst)) then lst
    else last(lst)::reverse(removeLast(lst))
end;


(* 2 *)
(* constructers *)
datatype term = Term of int * int * int;
datatype variable = Variable of string;
datatype poly = Poly of variable * variable * term list;

(* For example: 3x^3y + x^2 - 4y^2 + 2xy + 1 *)
(* Poly(Variable "x", Variable "y", [Term(3, 1, 3), Term(2, 0, 1), Term(0, 2, ~4), Term(1, 1, 2), Term(0, 0, 1)]) *)
(* Note that we assume x and y are two variables, and the 1st and 2nd elements of term are the exponentials of x and y, respectively. *)

(* Several functions you may find helpful for computations over polynomials *)
fun expon_x (Term(e, _, _)) = e;
fun expon_y (Term(_, e, _)) = e;
fun coeff (Term(_, _, c)) = c;

exception VariableMismatch;

fun diff_terms (l : term list, v : variable) : term list =
(* Your implementation here *)
    if null(l) then nil
    else if (expon_x(hd(l)) = 0 andalso expon_y(hd(l)) = 0) orelse coeff(hd(l)) = 0 then diff_terms(tl(l), v)
    else 
        let val diffedterm = 
            if v = Variable "x" then Term(expon_x(hd(l))-1, expon_y(hd(l)), expon_x(hd(l))*coeff(hd(l)))
                                else Term(expon_x(hd(l)), expon_y(hd(l))-1, expon_y(hd(l))*coeff(hd(l)))
        in
            if coeff(diffedterm) = 0 
            then diff_terms(tl(l), v)
            else diffedterm :: diff_terms(tl(l), v)
        end
;

fun diff_poly (Poly(xx, yy, l), v) : poly = 
  if (xx = v) orelse (yy = v) then 
    Poly (xx, yy, diff_terms (l, v))
  else
    raise VariableMismatch 
;




(* Part 2 *)
fun check_bull([a, b, c, d, e] : int list) : bool =
    let fun check_bull_3([a, b, c] : int list) : bool = (a + b + c) mod 10 = 0
        fun check_bull_4([a, b, c, d] : int list) : bool =
            check_bull_3([a, b, c]) orelse
            check_bull_3([a, b, d]) orelse
            check_bull_3([a, c, d]) orelse
            check_bull_3([b, c, d])
    in
        check_bull_4([a, b, c, d]) orelse
        check_bull_4([a, b, c, e]) orelse
        check_bull_4([a, b, d, e]) orelse
        check_bull_4([a, c, d, e]) orelse
        check_bull_4([b, c, d, e])
    end
;

fun get_point_bull(lst : int list) : int =
    let fun list_sum(nil) : int = 0
        |   list_sum(h::t) = h + list_sum(t)
        val remainder = list_sum(lst) mod 10
    in
        if remainder = 0 then 10
        else remainder
    end
;

fun get_point_non_bull(lst : int list) : int =
    let fun list_max(nil) : int = 0
        |   list_max(h::t) = 
                if h > list_max(t) then h
                else list_max(t)
    in
        list_max(lst)
    end
;

fun compare_result(lst1, lst2) : string =
    let val is_bull_1 = check_bull(lst1)
        val is_bull_2 = check_bull(lst2)
        val pt1 = if is_bull_1
                    then get_point_bull(lst1) 
                    else get_point_non_bull(lst1)
        val pt2 = if is_bull_2
                    then get_point_bull(lst2)
                    else get_point_non_bull(lst2)
    in
        if is_bull_1 andalso not is_bull_2 then "Player 1 wins"
        else if not is_bull_1 andalso is_bull_2 then "Player 2 wins"
        else if pt1 > pt2 then "Player 1 wins"
        else if pt1 < pt2 then "Player 2 wins"
        else "This is a tie"
    end
;
