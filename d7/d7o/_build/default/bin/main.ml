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

let process_q (curr: int64) (q: int64 list) (tgt:int64) : int64 list = 
        let rec aux (queue: int64 list) (acc: int64 list) : int64 list = 
                match queue with
                | hd:: tl -> (
                        let sum_check = Int64.(hd + curr <= tgt) in
                        let mul_check = Int64.(hd * curr <= tgt) in
                        match (sum_check, mul_check) with
                        | (true,true) -> aux tl (Int64.(hd * curr)::Int64.(hd + curr):: acc)
                        | (true,false)-> aux tl (Int64.(hd + curr)::acc)
                        | (false, true) -> aux tl (Int64.(hd * curr)::acc)
                        | (false, false) -> aux tl acc
                )
                | [] -> acc
        in 
        aux q []

let process_q_p2 (curr: int64) (q: int64 list) (tgt:int64) : int64 list = 
        let rec aux (queue: int64 list) (acc: int64 list) : int64 list = 
                match queue with
                | hd:: tl -> (
                        let sum_check = Int64.(hd + curr <= tgt) in
                        let mul_check = Int64.(hd * curr <= tgt) in
                        let concat_check = Int64.(Int64.of_string ((Int64.to_string hd) ^ (Int64.to_string curr)) <= tgt) in
                        match (sum_check, mul_check,concat_check) with
                        | true,true,false -> aux tl (Int64.(hd * curr)::Int64.(hd + curr):: acc)
                        | true,true,true -> aux tl (Int64.of_string ((Int64.to_string hd) ^ (Int64.to_string curr))::Int64.(hd * curr)::Int64.(hd + curr):: acc)
                        | true,false,false-> aux tl (Int64.(hd + curr)::acc)
                        | true,false,true-> aux tl (Int64.of_string ((Int64.to_string hd) ^ (Int64.to_string curr))::Int64.(hd + curr)::acc)
                        | false, true, false -> aux tl (Int64.(hd * curr)::acc)
                        | false, true, true -> aux tl (Int64.of_string ((Int64.to_string hd) ^ (Int64.to_string curr))::Int64.(hd * curr)::acc)
                        | false, false,false -> aux tl acc
                        | false, false,true -> aux tl (Int64.of_string ((Int64.to_string hd) ^ (Int64.to_string curr))::acc)
                )
                | [] -> acc
        in 
        aux q []


let process_line (line: int64 list) : int64 = 
    match line with
    | hd:: tl -> ( 
        let level (rest: int64 list) (q_init: int64 list) (tgt: int64) : int64 = 
                let rec aux (rem: int64 list) (queue: int64 list): int64 list =
                    match rem with
                    | curr::[] -> process_q curr queue hd
                    | curr :: remaining -> aux remaining (process_q curr queue tgt)
                    | _ -> assert false
                in
                let contains = List.mem (aux rest q_init) tgt ~equal: Int64.equal in
                match contains with
                |true -> tgt
                |false -> Int64.zero
        in
        match tl with 
        | front :: back -> level back [front] hd 
        | _ -> assert false
    )
    | _ -> assert false

let process_line_p2 (line: int64 list) : int64 = 
    match line with
    | hd:: tl -> ( 
        let level (rest: int64 list) (q_init: int64 list) (tgt: int64) : int64 = 
                let rec aux (rem: int64 list) (queue: int64 list): int64 list =
                    match rem with
                    | curr::[] -> process_q_p2 curr queue hd
                    | curr :: remaining -> aux remaining (process_q_p2 curr queue tgt)
                    | _ -> assert false
                in
                let contains = List.mem (aux rest q_init) tgt ~equal: Int64.equal in
                match contains with
                |true -> tgt
                |false -> Int64.zero
        in
        match tl with 
        | front :: back -> level back [front] hd 
        | _ -> assert false
    )
    | _ -> assert false
let () = 
        let filename = In_channel.input_line_exn In_channel.stdin in 
        let lines_list = read_lines filename in
        let result1: int64 = List.fold lines_list ~init:Int64.zero ~f:(fun acc line -> 
                Int64.( acc + (process_line line)) )
                in
        let result2: int64 = List.fold lines_list ~init:Int64.zero ~f:(fun acc line -> 
                Int64.( acc + (process_line_p2 line)) )
                in
        printf "Result p1: %Ld \n Result p2: %Ld \n" result1 result2


