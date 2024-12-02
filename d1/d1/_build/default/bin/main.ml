open Base
open Stdio

let read_lines filename =
  In_channel.with_file filename ~f:(fun in_channel ->
      In_channel.fold_lines in_channel ~init:[] ~f:(fun acc line ->
          match String.split line ~on:' ' |> List.filter ~f:(fun s -> not (String.is_empty s)) with
          | [first; second] -> (Int.of_string first, Int.of_string second) :: acc
          | _ -> failwith ("Invalid line format: " ^ line)))

let read_lines_p2 filename = 
  In_channel.with_file filename ~f:(fun in_channel -> 
      In_channel.fold_lines in_channel 
        ~init:([], Map.empty (module Int)) 
        ~f:(fun (list1, map) line -> 
            (* Split the line into parts and filter out empty strings *)
            match String.split line ~on:' ' |> List.filter ~f:(fun s -> not (String.is_empty s)) with
            | [first; second] -> (
                match Int.of_string_opt first, Int.of_string_opt second with
                | Some f, Some s -> 
                    let updated_map = 
                      Map.update map s ~f:(function 
                          | None -> 1
                          | Some count -> count + 1)
                    in
                    (f :: list1, updated_map)
                | _ -> failwith ("Invalid integers in line: " ^ line))
            | _ -> failwith ("Invalid line format: " ^ line)))

let () = 
        let filename = In_channel.input_line_exn In_channel.stdin in 

        let list1 , list2 = List.unzip (read_lines filename) in
        let list , map = read_lines_p2 filename in 
        let result1 = List.fold2_exn (List.sort ~compare:Int.compare list1) (List.sort ~compare:Int.compare list2) ~init: 0 ~f:(fun acc x y -> acc + Int.abs (x-y)) in 
        let result2 = List.fold list ~init: 0 ~f:(fun acc x -> 
                match Map.find map x with 
                | None -> acc
                | Some y -> acc + (x*y))
        in 
        printf "Result p1: %d\n Result p2: %d \n" result1 result2
        (* *) 

