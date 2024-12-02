open Base
open Stdio

let read_lines filename =
  In_channel.with_file filename ~f:(fun in_channel ->
      In_channel.fold_lines in_channel ~init:[] ~f:(fun acc line ->
        line :: acc))

let parsed_line line =
        match line with
        | first :: second :: _ when first > second -> 
                        let rec decreasing lst = 
                                match lst with
                                | [] -> 1
                                | _ :: [] -> 1
                                | fst :: scnd :: back when (fst > scnd && scnd >= fst - 3)  -> 
                                                decreasing (scnd :: back)
                                | _ -> 0
                        in 
                        decreasing line
        | first :: second :: _ when first < second -> 
                        let rec increasing lst = 
                                match lst with
                                | [] -> 1
                                | _ :: [] -> 1
                                | fst :: scnd :: back when (fst < scnd && scnd - 3 <= fst)  -> 
                                                increasing (scnd :: back)
                                | _ -> 0
                        in 
                        increasing line
        | _ :: [] -> 1
        | _ -> 0

let parsed_line_dampener line = 
        match parsed_line line with 
        | 1 -> 1
        | 0 -> (
                let rec dampener front back omitted= 
                        match back with
                        | hd :: tail -> (
                                        match parsed_line ( front @ (omitted :: tail) ) with 
                                        | 0 -> dampener (front @ [omitted]) tail hd
                                        | 1 -> 1
                                        | _ -> 2000
                                        )
                        | [] -> parsed_line front
                in
                match line with
                | hd :: tl -> dampener [] tl hd
                | _ ->  2000)
        | _ -> 2000

let () = 
        let filename = In_channel.input_line_exn In_channel.stdin in 
        let lines_list = read_lines filename in
        let result1 = List.fold lines_list ~init:0 ~f:(fun acc line -> 
                let line = List.map (String.split_on_chars  line ~on:[' ']) ~f:(fun x -> Int.of_string x) in
                acc + parsed_line line )
                in
        let result2 = List.fold lines_list ~init:0 ~f:(fun acc line -> 
                let line = List.map (String.split_on_chars  line ~on:[' ']) ~f:(fun x -> Int.of_string x) in
                acc + parsed_line_dampener line )
                in
        printf "Result p1: %d\n Result p2: %d \n" result1 result2


