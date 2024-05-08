module Node = struct
  type 'v t = { children : 'v t Js.Dict.t; mutable value : 'v option }

  let create () = { children = Js.Dict.empty (); value = None }

  let forEachChildrenImpl : 'v t Js.Dict.t -> (string -> 'v t -> unit) -> unit =
    [%mel.raw
      {|
      (children, f) => {
        for (let char in children) {
          f(char, children[char]);
        }
      }
    |}]

  let forEachChildren node = forEachChildrenImpl node.children
end

module Trie = struct
  type 'v t = { root : 'v Node.t }

  let create () = { root = Node.create () }

  let insert trie key value =
    let node = ref trie.root in

    let key = Js.String.toLowerCase key in
    let length = Js.String.length key in
    for index = 0 to length - 1 do
      let char = Js.String.charAt ~index key in
      match Js.Dict.get !node.children char with
      | None ->
          let childNode = Node.create () in
          Js.Dict.set !node.children char childNode;
          node := childNode
      | Some childNode -> node := childNode
    done;

    !node.value <- Some value

  exception NotFound

  let searchExact trie key =
    let node = ref trie.root in

    let key = Js.String.toLowerCase key in
    let length = Js.String.length key in
    try
      for index = 0 to length - 1 do
        let char = Js.String.charAt ~index key in
        match Js.Dict.get !node.children char with
        | None -> raise NotFound
        | Some childNode -> node := childNode
      done;
      !node.value
    with NotFound -> None

  let rec findPartialMatches (node : 'v Node.t) key values =
    let _ =
      match node.value with
      | Some value ->
          let _ = Js.Array.push ~value values in
          ()
      | None -> ()
    in
    Node.forEachChildren node (fun char childNode ->
        findPartialMatches childNode (key ^ char) values)

  let searchPartial trie key =
    let values = [||] in
    let node = ref trie.root in

    let key = Js.String.toLowerCase key in
    let length = Js.String.length key in
    try
      for index = 0 to length - 1 do
        let char = Js.String.charAt ~index key in
        match Js.Dict.get !node.children char with
        | None -> raise NotFound
        | Some childNode -> node := childNode
      done;
      findPartialMatches !node key values;
      values
    with NotFound -> [||]
end
