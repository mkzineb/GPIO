
(*
let gen_reg_name id =
  match Irg.get_symbol id with
  | Irg.REG(name, _, _, _) -> name
  | _ -> failwith "gen_reg_name: Invalid symbol"

let gen_reg_offset id =
  match Irg.get_symbol id with
  | Irg.REG(_, offset, _, _) -> offset
  | _ -> failwith "gen_reg_offset: Invalid symbol"

let gen_reg regs f dict =
  let new_dict r =
    let name = gen_reg_name r in
    let offset = gen_reg_offset r in
    ("name", Templater.TEXT (fun out -> output_string out name)) ::
    ("offset", Templater.TEXT (fun out -> output_string out offset)) ::
    dict
  in
  List.iter (fun r -> f (new_dict r)) regs


*)
  (*______________________________________________________________________________________________________________________________________*)
  Irg.load "../nmp/gpio.nmp" ;;
  let get_port_name =  Irg.get_proc_name () ;;

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


let dictionary = [
  (*("comp", Templater.TEXT (fun out -> Printf.fprintf out "%s" "gpio")) ;*)
  ("comp", Templater.TEXT (fun out -> Printf.fprintf out "%s" get_port_name)) ;
  ("compSec", Templater.TEXT (fun out -> Printf.fprintf out "%s" "TIM")) ;

  ("registersSec", Templater.COLL(gen_reg_ref ["last";"running";"CR1";"SR";"ARR";"CNT"] )) ;
  ("registers", Templater.COLL (gen_reg ["MODR";"IDR";"ODR"] ) )
]

(*creating both files from the dict*)
let _ =
  Templater.generate dictionary "template_gpio.c" "test_dict_gpio.c"  ;
  Templater.generate dictionary "template_gpio.h" "test_dict_gpio.h"  ;
