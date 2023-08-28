
  let gen_reg regs f dict =
  let new_dict r =
    let offset = 
      match r with
      | "MODR" -> "0x00"
      | "IDR" -> "0x10"
      | "ODR" -> "0x14"
      | _ -> failwith "Invalid register name"
    in
    ("name", Templater.TEXT (fun out -> output_string out r)) ::
   ("offset", Templater.TEXT (fun out -> output_string out offset)) ::
    dict
  in
  List.iter (fun r -> f (new_dict r)) regs


let gen_reg_ref regs f dict =
  let new_dict r =
    let offset = 
      match r with
      | "last" -> "0x00"
      | "running" -> "0x00"
      | "CR1" -> "0x00"
      | "SR" -> "0x10"
      | "ARR" -> "0x2C"
      | "CNT" -> "0x24" 
      | _ -> failwith "Invalid register name"
    in
    ("nameSec", Templater.TEXT (fun out -> output_string out r)) ::
    ("offsetSec", Templater.TEXT (fun out -> output_string out offset)) ::
    dict
  in
  List.iter (fun r -> f (new_dict r)) regs
	
let getreg f dict =
    Irg.iter
      (fun name spec ->
        match spec with
        | Irg.REG(_, size, typ, atts) ->
            f (
              ("name", Templater.TEXT (fun out -> output_string out name))
              ::
              ("offset", Templater.TEXT (fun out -> Printf.fprintf out "0x%X" size; flush out ))
              ::
              ("on_input", Templater.TEXT (fun out ->
			let action = Irg.attr_stat "on_input" atts Irg.NOP in
			let info = Toc.info () in
			info.Toc.out <- out;
			Toc.gen_stat info (Toc.prepare_stat info action)))
	      ::
	      
              dict)
        |  _ -> ())



let getcomp f = 
Irg.iter
      (fun name spec ->
        match spec with
  	Irg.LET(_, _, _, _) ->
  	f (
    	("name", Templater.TEXT (fun out -> output_string out name))::
            [])
        | _ -> ())
      
     

let getport f dict =
    Irg.iter
      (fun name spec ->
        match spec with
        | Irg.PORT(_, size, typ, atts) ->
            f (
              ("name", Templater.TEXT (fun out -> output_string out name))::
              ("size", Templater.TEXT(fun out -> output_string out name))::
              ("on_input", Templater.TEXT (fun out ->
			let action = Irg.attr_stat "on_input" atts Irg.NOP in
			let info = Toc.info () in
			info.Toc.out <- out;
			Toc.gen_stat info (Toc.prepare_stat info action)        	
              ))::
              dict)
        |  _ -> ())
        



let dictionary = [
  ("comp", Templater.TEXT (fun out -> Printf.fprintf out "%s" "gpio" )) ;
  ("registersSec", Templater.COLL(gen_reg_ref ["last";"running";"CR1";"SR";"ARR";"CNT"] )) ;
  ("registers", Templater.COLL getreg)
]
 
 



(* main program *)
let _  =
	App.run
		[]
		""
		(fun info ->
			Templater.generate dictionary "template_gpio.c" "test_dict_gpio.c"  ;
  			Templater.generate dictionary "template_gpio.h" "test_dict_gpio.h")
			
