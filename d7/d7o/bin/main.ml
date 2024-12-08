open Base
open Stdio

let read_lines filename =
  In_channel.with_file filename ~f:(fun in_channel ->
      In_channel.fold_lines in_channel ~init:[] ~f:(fun acc line ->
              let line_list = String.split_on_chars line ~on:[':'; ' '] 
                      |> List.filter ~f:(fun s -> not (String.is_empty s)) in
              let int_list = List.map line_list ~f:(fun x -> Int64.of_string x) in
              int_list::acc
      ))

open Base

let process_line (line: int64 list) : int64 = 
    match line with
    | hd:: tl -> (
        let rec level rest q_last tgt acc = 
            match q_last, acc with 
            | [],[] -> Int64.zero
            | _ -> (
                match rest with 
                | curr :: [] -> (
                    match q_last with
                    | [] -> Int64.zero
                    | q_acc :: q_rest -> (
                        let sum_check = Int64.(q_acc + curr <= tgt) in
                        let mul_check = Int64.(q_acc * curr <= tgt) in
                        match (sum_check, mul_check) with
                        | true, _ -> tgt
                        | _, true -> tgt
                        | _ -> level q_rest [] tgt []
                    )
                )
                | curr :: back -> (
                    match q_last with 
                    | [] -> level back acc tgt []
                    | q_acc :: q_rest -> (
                        let sum_check = Int64.(q_acc + curr <= tgt) in
                        let mul_check = Int64.(q_acc * curr <= tgt) in
                        match (sum_check, mul_check) with
                        | (true,true) -> level back q_rest tgt (Int64.(q_acc * curr)::Int64.(q_acc + curr):: acc)
                        | (true,false)-> level back q_rest tgt (Int64.(q_acc + curr)::acc)
                        | (false, true) -> level back q_rest tgt (Int64.(q_acc * curr)::acc)
                        | (false, false) -> level back q_rest tgt acc
                    )
                )
                | _ -> assert false
            )
        in
        match tl with 
        | front :: back -> level back [front] hd []
        | _ -> assert false
    )
    | _ -> assert false

let () = 
        let filename = In_channel.input_line_exn In_channel.stdin in 
        let lines_list = read_lines filename in
        let result1: int64 = List.fold lines_list ~init:Int64.zero ~f:(fun acc line -> 
                Int64.( acc + (process_line line)) )
                in
        (*let result2 = List.fold lines_list ~init:0 ~f:(fun acc line -> 
                let line = List.map (String.split_on_chars  line ~on:[' ']) ~f:(fun x -> Int.of_string x) in
                acc + parsed_line_dampener line )
                in*)
        printf "Result p1: %Ld \n" result1 



