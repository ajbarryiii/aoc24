open Base
open Stdio

let read_lines filename  = 
        In_channel.wih_file filename ~f:(fun in_channel -> 
                In_channel.fold_lines in_channel ~init: ([], []) ~f:(fun (list1, list2) line -> 
                        let first, second = String.lsplit2_exn ~on: '\t' line in  
                        ((Int.of_string first) :: list1 , (Int.of_string second) :: list2)))

let read_lines_p2 filename = 
        In_channel.wih_file filename ~f:(fun in_channel -> 
                In_channel.fold_lines in_channel ~init: ([], Map.empty (module Int) ) ~f:(fun (list1, map) line -> 
                        match String.lsplit2_exn ~on: '\t' line with
                        | Some (first, second) -> (
                                match Int.of_string first, Int.of_string second with
                                | Some f, Some s -> 
                                                let updated_map = Map.update map s ~f: (function 
                                                        | None -> 1
                                                        | Some count -> count + 1)
                                                in 
                                                (f :: list1 , updated_map)
                                | _ -> 
                                        printf "Error: Invalid integers";
                                        exit 1)
                        | _ -> 
                                printf "Error: Invalid lines"; 
                                exit 1))

let () = 
        printf "Enter the filename: ";
        let filename = In_channel.input_line_exn In_channel.stdin in 

        let list1 , list2 = read_lines filename in
        let list , map = read_lines_p2 filename in 
        let result1 = List.fold2_exn (List.sort list1) (List.sort list2) ~init: 0 ~f:(fun acc x y -> acc + Int.abs (x-y)) in 
        let result2 = List.fold list ~init: 0 ~f:(fun acc x -> acc + (x * (Map.find map x))) in 
        printf "Result p1: %d\n Result p2: %d \n" result1 result2
        (* *) 
