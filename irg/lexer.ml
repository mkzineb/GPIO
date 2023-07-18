# 21 "lexer.mll"
 

open Parser
open Lexing
(*exception BadChar of char
exception BadLine*)

(* Line count *)
let file = ref ""
let line = ref 1
let line_offset = ref 0
let bitfld = ref false
let lexbuf = ref (Lexing.from_string "")

(* Keyword detection *)
let lexicon = Irg.StringHashtbl.create 211
let keyword id =
	try
		let e=Irg.StringHashtbl.find lexicon id
		in
		match e with
		 EXCEPTION _->EXCEPTION !line
		|LET _->LET !line
		|MEM _->MEM !line
		|MODE _->MODE !line
		|PORT _->PORT !line
		|OP _-> OP !line
		|REG _->REG !line
		|VAR _->VAR !line
		|RESOURCE _->RESOURCE !line
		|TYPE _->TYPE !line
		|_->e
	with Not_found -> (ID id)

let keywords = [
	("__attr",		ATTR);
	("action",      ACTION);
	("alias",       ALIAS);
	("bool",        BOOL);
	("canon",		CANON);
	("card",        CARD);
	("case",        CASE);
	("coerce",      COERCE);
	("default",     DEFAULT);
	("do",			DO);
	("else",        ELSE);
	("elsif",	ELSIF);
	("enddo",		ENDDO);
	("endif",       ENDIF);
	("enum",        ENUM);
	("error",       ERROR);
	("exception",   EXCEPTION 0);
	("extend",		EXTEND);
	("fix",        	FIX);
	("float",       FLOAT);
	("for",			FOR);
	("format",      FORMAT);
	("if",        	IF);
	("image",       IMAGE);
	("in",			IN);
	("initial",     INITIALA);
	("int",        	INT);
	("let",        	LET 0);
	("macro",       MACRO);
	("mem",        	MEM 0);
	("mode",        MODE 0);
	("not",        	NOT);
	("op",        	OP 0);
	("reg",        	REG 0);
	("port",	PORT 0);
	("var",        	VAR 0);
	("resource",    RESOURCE 0);
	("syntax",      SYNTAX);
	("switch",      SWITCH);
	("then",        THEN);
	("type",        TYPE 0);
	("uses",        USES);
	("volatile",    VOLATILE)
]

(* Is the NOP keyword really required ?
	("nop",        	NOP);
*)


let _ =
	let add (key, token) = Irg.StringHashtbl.add lexicon key token in
	Irg.StringHashtbl.clear lexicon;
	List.iter add keywords


(** Compute column of the current symbol.
	@return		Column number. *)
let get_col _ =
	(Lexing.lexeme_start !lexbuf) - !line_offset + 1


(** Raise an error with the given function message at the current
	position in the lexing.
	@param m	Function of the message. *)
let error m =
	let f, l, c = !file, !line, get_col () in
	raise (Irg.Error (fun out ->
		Printf.fprintf out "%s:%d:%d: " f l c;
		m out))


(** Output an error on the given stream with the current (file, line, column) position.
	@param out	Output channel.
	@param msg	Message to output. *)
let output_error out msg =
	Printf.fprintf out "ERROR: %s:%d:%d: %s\n" !file !line (get_col ()) msg


(** Output an error with the current (file, line, column) position.
	@param msg	Message to output. *)
let display_error msg = output_error stderr msg


(** Get the current location as a string.
	@return	Current location string. *)
let current_loc _ = Printf.sprintf "%s:%d:%d" !file !line (get_col ())


(** Display as a warning the message produced by the given function.
	@param f	Function to produce the display. *)
let warn f =
	Printf.fprintf stderr "WARNING: %s:%d:%d: %t\n" !file !line (get_col ()) f


(** warning management
	@param msg		Message to display. *)
let display_warning msg =
	warn (fun out -> output_string out msg)


(* Lexing add-ons *)
let rec dotdot lexbuf i found =
	if i >= lexbuf.lex_buffer_len then
		if lexbuf.lex_eof_reached then false
		else begin
			let diff = i - lexbuf.lex_start_pos in
			lexbuf.refill_buff lexbuf;
			dotdot lexbuf (lexbuf.lex_start_pos + diff) found
		end
	else
		(*match lexbuf.lex_buffer.[i] with*)
		match Bytes.get lexbuf.lex_buffer i with
		  '\n' -> false
		| '.' -> if found then true else dotdot lexbuf (i + 1) true
		| '<' | '>' | '=' | ';' | '}' -> false
		| _ -> dotdot lexbuf (i + 1) false

let gt lexbuf token size =
	if not !bitfld then token
	else begin
		bitfld := false;
		lexbuf.lex_curr_pos <- lexbuf.lex_curr_pos - size;
		GT
	end

let append s c = s ^ (String.make 1 c)

let appends s c =
	let c =
		match c with
		| 'n' -> '\n'
		| 't' -> '\t'
		| 'r' -> '\r'
		| c -> c in
	append s c


(** Record a new line. *)
let new_line lexbuf =
	incr line;
	line_offset := Lexing.lexeme_end lexbuf


(** Raise an error at current position because a bad character is found. *)
let bad_char c =
	error (fun out -> Printf.fprintf out "unsupported character: '%c'" c)


(** Raise an error at the current position because a #line is malformed. *)
let bad_line _ =
	error (Irg.asis "malformed #line")


# 192 "lexer.ml"
let __ocaml_lex_tables = {
  Lexing.lex_base =
   "\000\000\204\255\205\255\206\255\207\255\208\255\209\255\212\255\
    \213\255\214\255\215\255\216\255\217\255\218\255\219\255\220\255\
    \221\255\229\255\003\000\025\000\031\000\035\000\086\000\071\000\
    \084\000\071\000\072\000\087\000\164\000\186\000\037\000\250\255\
    \251\255\104\000\254\255\255\255\252\255\253\255\042\000\038\000\
    \048\000\249\255\240\000\174\000\130\000\003\001\026\001\132\000\
    \060\001\087\001\097\001\114\001\124\001\134\001\237\255\088\000\
    \244\255\232\255\123\000\243\255\240\255\239\255\238\255\236\255\
    \235\255\234\255\233\255\001\000\253\255\254\255\255\255\234\000\
    \252\255\253\255\254\255\164\000\255\255\211\000\252\255\253\255\
    \002\000\255\255\254\255\207\000\253\255\003\000\255\255\254\255\
    \187\001\252\255\253\255\254\255\172\001\010\001\252\255\253\255\
    \254\255\255\255";
  Lexing.lex_backtrk =
   "\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\031\000\030\000\033\000\028\000\044\000\032\000\
    \029\000\024\000\025\000\010\000\007\000\007\000\027\000\255\255\
    \255\255\045\000\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\007\000\008\000\
    \009\000\255\255\009\000\255\255\009\000\255\255\255\255\013\000\
    \255\255\255\255\014\000\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\002\000\255\255\255\255\255\255\255\255\
    \002\000\255\255\255\255\255\255\255\255\002\000\255\255\255\255\
    \255\255\255\255\255\255\255\255\000\000\255\255\255\255\255\255\
    \255\255\255\255";
  Lexing.lex_default =
   "\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\000\000\
    \000\000\255\255\000\000\000\000\000\000\000\000\255\255\255\255\
    \255\255\000\000\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\000\000\255\255\
    \000\000\000\000\255\255\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\069\000\000\000\000\000\000\000\073\000\
    \000\000\000\000\000\000\255\255\000\000\079\000\000\000\000\000\
    \082\000\000\000\000\000\084\000\000\000\087\000\000\000\000\000\
    \090\000\000\000\000\000\000\000\255\255\095\000\000\000\000\000\
    \000\000\000\000";
  Lexing.lex_trans =
   "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\035\000\034\000\070\000\000\000\035\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \035\000\020\000\032\000\030\000\017\000\006\000\019\000\031\000\
    \014\000\013\000\022\000\008\000\015\000\007\000\024\000\033\000\
    \029\000\028\000\028\000\028\000\028\000\028\000\028\000\028\000\
    \028\000\028\000\023\000\016\000\025\000\021\000\026\000\065\000\
    \003\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\012\000\064\000\011\000\004\000\027\000\
    \063\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\010\000\018\000\009\000\005\000\066\000\
    \062\000\061\000\060\000\058\000\057\000\054\000\055\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\038\000\036\000\039\000\040\000\041\000\056\000\037\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\047\000\047\000\047\000\047\000\027\000\059\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\043\000\076\000\028\000\028\000\028\000\028\000\
    \028\000\028\000\028\000\028\000\028\000\028\000\048\000\048\000\
    \048\000\048\000\048\000\048\000\048\000\048\000\048\000\048\000\
    \043\000\042\000\028\000\028\000\028\000\028\000\028\000\028\000\
    \028\000\028\000\028\000\028\000\074\000\081\000\086\000\000\000\
    \000\000\000\000\000\000\000\000\044\000\000\000\000\000\042\000\
    \002\000\068\000\255\255\255\255\000\000\000\000\000\000\000\000\
    \000\000\042\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\045\000\097\000\075\000\000\000\000\000\097\000\
    \000\000\000\000\000\000\053\000\044\000\053\000\000\000\042\000\
    \052\000\052\000\052\000\052\000\052\000\052\000\052\000\052\000\
    \052\000\052\000\097\000\085\000\096\000\000\000\000\000\080\000\
    \000\000\000\000\045\000\046\000\046\000\046\000\046\000\046\000\
    \046\000\046\000\046\000\046\000\046\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\046\000\046\000\046\000\046\000\
    \046\000\046\000\046\000\046\000\046\000\046\000\046\000\046\000\
    \046\000\046\000\046\000\046\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\046\000\046\000\046\000\046\000\046\000\
    \046\000\000\000\000\000\000\000\046\000\046\000\046\000\046\000\
    \046\000\046\000\000\000\000\000\048\000\048\000\048\000\048\000\
    \048\000\048\000\048\000\048\000\048\000\048\000\000\000\000\000\
    \000\000\000\000\000\000\046\000\046\000\046\000\046\000\046\000\
    \046\000\049\000\051\000\000\000\051\000\000\000\000\000\050\000\
    \050\000\050\000\050\000\050\000\050\000\050\000\050\000\050\000\
    \050\000\050\000\050\000\050\000\050\000\050\000\050\000\050\000\
    \050\000\050\000\050\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\049\000\050\000\050\000\050\000\050\000\050\000\050\000\
    \050\000\050\000\050\000\050\000\052\000\052\000\052\000\052\000\
    \052\000\052\000\052\000\052\000\052\000\052\000\052\000\052\000\
    \052\000\052\000\052\000\052\000\052\000\052\000\052\000\052\000\
    \000\000\000\000\000\000\000\000\091\000\000\000\000\000\000\000\
    \091\000\000\000\000\000\000\000\000\000\000\000\000\000\255\255\
    \000\000\000\000\000\000\078\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\091\000\092\000\092\000\092\000\092\000\
    \092\000\092\000\092\000\092\000\092\000\092\000\000\000\000\000\
    \000\000\000\000\072\000\092\000\092\000\092\000\092\000\092\000\
    \092\000\092\000\092\000\092\000\092\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\094\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\089\000";
  Lexing.lex_check =
   "\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\000\000\000\000\067\000\255\255\000\000\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\019\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\020\000\000\000\000\000\000\000\
    \021\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
    \000\000\000\000\000\000\000\000\000\000\000\000\000\000\018\000\
    \022\000\023\000\024\000\025\000\025\000\026\000\026\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\030\000\033\000\038\000\039\000\040\000\055\000\033\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\044\000\044\000\047\000\047\000\027\000\058\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\027\000\027\000\027\000\027\000\027\000\027\000\
    \027\000\027\000\028\000\075\000\028\000\028\000\028\000\028\000\
    \028\000\028\000\028\000\028\000\028\000\028\000\043\000\043\000\
    \043\000\043\000\043\000\043\000\043\000\043\000\043\000\043\000\
    \029\000\028\000\029\000\029\000\029\000\029\000\029\000\029\000\
    \029\000\029\000\029\000\029\000\071\000\077\000\083\000\255\255\
    \255\255\255\255\255\255\255\255\029\000\255\255\255\255\029\000\
    \000\000\067\000\080\000\085\000\255\255\255\255\255\255\255\255\
    \255\255\028\000\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\029\000\093\000\071\000\255\255\255\255\093\000\
    \255\255\255\255\255\255\042\000\029\000\042\000\255\255\029\000\
    \042\000\042\000\042\000\042\000\042\000\042\000\042\000\042\000\
    \042\000\042\000\093\000\083\000\093\000\255\255\255\255\077\000\
    \255\255\255\255\029\000\045\000\045\000\045\000\045\000\045\000\
    \045\000\045\000\045\000\045\000\045\000\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\045\000\045\000\045\000\045\000\
    \045\000\045\000\046\000\046\000\046\000\046\000\046\000\046\000\
    \046\000\046\000\046\000\046\000\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\046\000\046\000\046\000\046\000\046\000\
    \046\000\255\255\255\255\255\255\045\000\045\000\045\000\045\000\
    \045\000\045\000\255\255\255\255\048\000\048\000\048\000\048\000\
    \048\000\048\000\048\000\048\000\048\000\048\000\255\255\255\255\
    \255\255\255\255\255\255\046\000\046\000\046\000\046\000\046\000\
    \046\000\048\000\049\000\255\255\049\000\255\255\255\255\049\000\
    \049\000\049\000\049\000\049\000\049\000\049\000\049\000\049\000\
    \049\000\050\000\050\000\050\000\050\000\050\000\050\000\050\000\
    \050\000\050\000\050\000\255\255\255\255\255\255\255\255\255\255\
    \255\255\048\000\051\000\051\000\051\000\051\000\051\000\051\000\
    \051\000\051\000\051\000\051\000\052\000\052\000\052\000\052\000\
    \052\000\052\000\052\000\052\000\052\000\052\000\053\000\053\000\
    \053\000\053\000\053\000\053\000\053\000\053\000\053\000\053\000\
    \255\255\255\255\255\255\255\255\088\000\255\255\255\255\255\255\
    \088\000\255\255\255\255\255\255\255\255\255\255\255\255\083\000\
    \255\255\255\255\255\255\077\000\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\088\000\092\000\092\000\092\000\092\000\
    \092\000\092\000\092\000\092\000\092\000\092\000\255\255\255\255\
    \255\255\255\255\071\000\088\000\088\000\088\000\088\000\088\000\
    \088\000\088\000\088\000\088\000\088\000\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\093\000\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
    \255\255\255\255\255\255\088\000";
  Lexing.lex_base_code =
   "";
  Lexing.lex_backtrk_code =
   "";
  Lexing.lex_default_code =
   "";
  Lexing.lex_trans_code =
   "";
  Lexing.lex_check_code =
   "";
  Lexing.lex_code =
   "";
}

let rec main lexbuf =
   __ocaml_lex_main_rec lexbuf 0
and __ocaml_lex_main_rec lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
# 233 "lexer.mll"
        ( main lexbuf )
# 435 "lexer.ml"

  | 1 ->
# 234 "lexer.mll"
           ( new_line lexbuf; main lexbuf )
# 440 "lexer.ml"

  | 2 ->
# 235 "lexer.mll"
        ( eof_comment lexbuf )
# 445 "lexer.ml"

  | 3 ->
# 236 "lexer.mll"
        ( comment lexbuf )
# 450 "lexer.ml"

  | 4 ->
# 238 "lexer.mll"
        ( STRING_CONST  (str "" lexbuf) )
# 455 "lexer.ml"

  | 5 ->
# 239 "lexer.mll"
        ( chr "" lexbuf )
# 460 "lexer.ml"

  | 6 ->
# 240 "lexer.mll"
           ( scan_line lexbuf; scan_file lexbuf; main lexbuf )
# 465 "lexer.ml"

  | 7 ->
let
# 242 "lexer.mll"
        v
# 471 "lexer.ml"
= Lexing.sub_lexeme lexbuf lexbuf.Lexing.lex_start_pos lexbuf.Lexing.lex_curr_pos in
# 242 "lexer.mll"
            (	try CARD_CONST (Int32.of_string v)
					with Failure _-> CARD_CONST_64 (Int64.of_string v) )
# 476 "lexer.ml"

  | 8 ->
let
# 245 "lexer.mll"
            v
# 482 "lexer.ml"
= Lexing.sub_lexeme lexbuf lexbuf.Lexing.lex_start_pos lexbuf.Lexing.lex_curr_pos in
# 245 "lexer.mll"
               (	let size = (String.length v) - 2 in
				try(
					BIN_CONST (Int32.of_string v, size)
				)with Failure _-> BIN_CONST_64 (Int64.of_string v, size)
			)
# 490 "lexer.ml"

  | 9 ->
let
# 253 "lexer.mll"
         v
# 496 "lexer.ml"
= Lexing.sub_lexeme lexbuf lexbuf.Lexing.lex_start_pos lexbuf.Lexing.lex_curr_pos in
# 253 "lexer.mll"
           ( FIXED_CONST (float_of_string v) )
# 500 "lexer.ml"

  | 10 ->
let
# 254 "lexer.mll"
        v
# 506 "lexer.ml"
= Lexing.sub_lexeme lexbuf lexbuf.Lexing.lex_start_pos lexbuf.Lexing.lex_curr_pos in
# 254 "lexer.mll"
           (  keyword v )
# 510 "lexer.ml"

  | 11 ->
# 255 "lexer.mll"
         ( gt lexbuf ROTATE_RIGHT 2 )
# 515 "lexer.ml"

  | 12 ->
# 256 "lexer.mll"
         ( ROTATE_LEFT )
# 520 "lexer.ml"

  | 13 ->
# 257 "lexer.mll"
        ( gt lexbuf RIGHT_SHIFT 1 )
# 525 "lexer.ml"

  | 14 ->
# 258 "lexer.mll"
        ( LEFT_SHIFT )
# 530 "lexer.ml"

  | 15 ->
# 259 "lexer.mll"
        ( DOUBLE_DOT )
# 535 "lexer.ml"

  | 16 ->
# 260 "lexer.mll"
        ( DOUBLE_COLON )
# 540 "lexer.ml"

  | 17 ->
# 261 "lexer.mll"
        ( DOUBLE_STAR )
# 545 "lexer.ml"

  | 18 ->
# 262 "lexer.mll"
        ( gt lexbuf GEQ 1 )
# 550 "lexer.ml"

  | 19 ->
# 263 "lexer.mll"
        ( EQU )
# 555 "lexer.ml"

  | 20 ->
# 264 "lexer.mll"
        ( NEQ )
# 560 "lexer.ml"

  | 21 ->
# 265 "lexer.mll"
        ( AND )
# 565 "lexer.ml"

  | 22 ->
# 266 "lexer.mll"
        ( OR )
# 570 "lexer.ml"

  | 23 ->
# 267 "lexer.mll"
        ( LEQ )
# 575 "lexer.ml"

  | 24 ->
# 268 "lexer.mll"
       (
			 if dotdot lexbuf lexbuf.lex_last_pos false
				then
					begin bitfld := true; BIT_LEFT end
				else
					LT
			)
# 586 "lexer.ml"

  | 25 ->
# 275 "lexer.mll"
           ( bitfld := false; GT )
# 591 "lexer.ml"

  | 26 ->
# 276 "lexer.mll"
           ( DOLLAR )
# 596 "lexer.ml"

  | 27 ->
# 277 "lexer.mll"
       ( SHARP )
# 601 "lexer.ml"

  | 28 ->
# 278 "lexer.mll"
       ( EQ )
# 606 "lexer.ml"

  | 29 ->
# 279 "lexer.mll"
       ( DOT )
# 611 "lexer.ml"

  | 30 ->
# 280 "lexer.mll"
       ( AMPERS )
# 616 "lexer.ml"

  | 31 ->
# 281 "lexer.mll"
       ( PIPE )
# 621 "lexer.ml"

  | 32 ->
# 282 "lexer.mll"
       ( COLON )
# 626 "lexer.ml"

  | 33 ->
# 283 "lexer.mll"
       ( EXCLAM )
# 631 "lexer.ml"

  | 34 ->
# 284 "lexer.mll"
       ( SEMI )
# 636 "lexer.ml"

  | 35 ->
# 285 "lexer.mll"
       ( COMMA )
# 641 "lexer.ml"

  | 36 ->
# 286 "lexer.mll"
       ( LPAREN )
# 646 "lexer.ml"

  | 37 ->
# 287 "lexer.mll"
       ( RPAREN )
# 651 "lexer.ml"

  | 38 ->
# 288 "lexer.mll"
       ( LBRACK )
# 656 "lexer.ml"

  | 39 ->
# 289 "lexer.mll"
       ( RBRACK )
# 661 "lexer.ml"

  | 40 ->
# 290 "lexer.mll"
       ( LBRACE )
# 666 "lexer.ml"

  | 41 ->
# 291 "lexer.mll"
       ( RBRACE )
# 671 "lexer.ml"

  | 42 ->
# 292 "lexer.mll"
       ( PLUS )
# 676 "lexer.ml"

  | 43 ->
# 293 "lexer.mll"
       ( MINUS )
# 681 "lexer.ml"

  | 44 ->
# 294 "lexer.mll"
       ( STAR )
# 686 "lexer.ml"

  | 45 ->
# 295 "lexer.mll"
       ( SLASH )
# 691 "lexer.ml"

  | 46 ->
# 296 "lexer.mll"
       ( PERCENT )
# 696 "lexer.ml"

  | 47 ->
# 297 "lexer.mll"
       ( TILD )
# 701 "lexer.ml"

  | 48 ->
# 298 "lexer.mll"
       ( CIRC )
# 706 "lexer.ml"

  | 49 ->
# 299 "lexer.mll"
       ( AROBAS )
# 711 "lexer.ml"

  | 50 ->
# 301 "lexer.mll"
       ( EOF )
# 716 "lexer.ml"

  | 51 ->
let
# 302 "lexer.mll"
       v
# 722 "lexer.ml"
= Lexing.sub_lexeme_char lexbuf lexbuf.Lexing.lex_start_pos in
# 302 "lexer.mll"
         ( bad_char v )
# 726 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_main_rec lexbuf __ocaml_lex_state

and eof_comment lexbuf =
   __ocaml_lex_eof_comment_rec lexbuf 67
and __ocaml_lex_eof_comment_rec lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
# 306 "lexer.mll"
      ( new_line lexbuf; main lexbuf )
# 738 "lexer.ml"

  | 1 ->
# 307 "lexer.mll"
     ( eof_comment lexbuf )
# 743 "lexer.ml"

  | 2 ->
# 308 "lexer.mll"
       ( main lexbuf )
# 748 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_eof_comment_rec lexbuf __ocaml_lex_state

and comment lexbuf =
   __ocaml_lex_comment_rec lexbuf 71
and __ocaml_lex_comment_rec lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
# 312 "lexer.mll"
      ( main lexbuf )
# 760 "lexer.ml"

  | 1 ->
# 314 "lexer.mll"
       ( new_line lexbuf; comment lexbuf )
# 765 "lexer.ml"

  | 2 ->
# 315 "lexer.mll"
     ( comment lexbuf )
# 770 "lexer.ml"

  | 3 ->
# 316 "lexer.mll"
       ( error (Irg.asis "end-of-file found inside a comment") )
# 775 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_comment_rec lexbuf __ocaml_lex_state

and str res lexbuf =
   __ocaml_lex_str_rec res lexbuf 77
and __ocaml_lex_str_rec res lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
# 320 "lexer.mll"
        ( res )
# 787 "lexer.ml"

  | 1 ->
let
# 321 "lexer.mll"
             v
# 793 "lexer.ml"
= Lexing.sub_lexeme_char lexbuf (lexbuf.Lexing.lex_start_pos + 1) in
# 321 "lexer.mll"
                ( str (appends res v) lexbuf )
# 797 "lexer.ml"

  | 2 ->
let
# 322 "lexer.mll"
       v
# 803 "lexer.ml"
= Lexing.sub_lexeme_char lexbuf lexbuf.Lexing.lex_start_pos in
# 322 "lexer.mll"
           ( str (append res v) lexbuf )
# 807 "lexer.ml"

  | 3 ->
# 323 "lexer.mll"
         ( error (Irg.asis "end-of-file found inside a string") )
# 812 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_str_rec res lexbuf __ocaml_lex_state

and chr res lexbuf =
   __ocaml_lex_chr_rec res lexbuf 83
and __ocaml_lex_chr_rec res lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
# 327 "lexer.mll"
        ( STRING_CONST res )
# 824 "lexer.ml"

  | 1 ->
let
# 328 "lexer.mll"
             v
# 830 "lexer.ml"
= Lexing.sub_lexeme_char lexbuf (lexbuf.Lexing.lex_start_pos + 1) in
# 328 "lexer.mll"
                ( chr (appends res v) lexbuf )
# 834 "lexer.ml"

  | 2 ->
let
# 329 "lexer.mll"
       v
# 840 "lexer.ml"
= Lexing.sub_lexeme_char lexbuf lexbuf.Lexing.lex_start_pos in
# 329 "lexer.mll"
           ( chr (append res v) lexbuf )
# 844 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_chr_rec res lexbuf __ocaml_lex_state

and scan_line lexbuf =
   __ocaml_lex_scan_line_rec lexbuf 88
and __ocaml_lex_scan_line_rec lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
let
# 332 "lexer.mll"
           l
# 857 "lexer.ml"
= Lexing.sub_lexeme lexbuf lexbuf.Lexing.lex_start_pos lexbuf.Lexing.lex_curr_pos in
# 332 "lexer.mll"
             ( line := (int_of_string l) - 1 )
# 861 "lexer.ml"

  | 1 ->
# 333 "lexer.mll"
         ( scan_line lexbuf )
# 866 "lexer.ml"

  | 2 ->
# 334 "lexer.mll"
      ( bad_line () )
# 871 "lexer.ml"

  | 3 ->
# 335 "lexer.mll"
        ( bad_line () )
# 876 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_scan_line_rec lexbuf __ocaml_lex_state

and scan_file lexbuf =
   __ocaml_lex_scan_file_rec lexbuf 93
and __ocaml_lex_scan_file_rec lexbuf __ocaml_lex_state =
  match Lexing.engine __ocaml_lex_tables __ocaml_lex_state lexbuf with
      | 0 ->
# 338 "lexer.mll"
        ( scan_file lexbuf )
# 888 "lexer.ml"

  | 1 ->
# 339 "lexer.mll"
        ( file := (str "" lexbuf) )
# 893 "lexer.ml"

  | 2 ->
# 340 "lexer.mll"
      ( bad_line () )
# 898 "lexer.ml"

  | 3 ->
# 341 "lexer.mll"
        ( bad_line () )
# 903 "lexer.ml"

  | __ocaml_lex_state -> lexbuf.Lexing.refill_buff lexbuf;
      __ocaml_lex_scan_file_rec lexbuf __ocaml_lex_state

;;

# 344 "lexer.mll"
 

(** Consume tokens until finding a top-level else or endif.
	@param	f	Function to call in case of error.
	@return		True if an endif is found, false if an else is found. *)
let consume_then f =
	let rec consume n =
		match main !lexbuf with
		| IF ->
			consume (n + 1)
		| ENDIF when n = 0 ->
			true
		| ENDIF ->
			consume (n - 1)
		| ELSE when n = 0 ->
			false
		| ELSIF when n = 0 ->
			false
		| EOF ->
			f "End of file reached: unclosed if!"
		| _ ->
			consume n in
	consume 0


(** Consume tokens until finding a top-level endif.
	@param f	Function to call if an error araises. *)
let consume_else f =
	let rec consume n =
		match main !lexbuf with
		| IF ->
			consume (n + 1)
		| ENDIF when n = 0 ->
			()
		| ENDIF ->
			consume (n - 1)
		| ELSE when n = 0 ->
			f "Illegal 'else'."
		| ELSIF when n = 0 ->
			f "Illegal 'elsif'."
		| EOF ->
			f "End of file reached: unclosed if!"
		| _ ->
			consume n in
	consume 0

let consume_elsif f = 
    let rec consume n = 
        match main !lexbuf with
        | IF -> consume (n + 1)
        | ENDIF when n = 0 -> ()
        | ENDIF -> consume (n - 1)
        | ELSE when n = 0 -> ()
        | ELSIF when n = 0 -> ()
        | EOF -> f "End of file reached: unclosed if!"
        | _ -> consume n in
    consume 0

# 969 "lexer.ml"
