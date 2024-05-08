(** Efficient partial lookups for strings. *)

(** A search tree used for partial lookups. *)
module Trie : sig
  type 'v t
  (** A string trie with values of type ['v]. *)

  val create : unit -> 'v t
  (** [create ()] creates a new {!Trie.t} *)

  val insert : 'v t -> string -> 'v -> unit
  (** [insert trie key value] inserts the [key] / [value] pair into the [trie]. *)

  val searchExact : 'v t -> string -> 'v option
  (** [searchExact trie key] searches for an exact match of the [key] in the [trie].
        
      Example:

      {@ocaml[
        let trie = Trie.create () in
        Trie.insert trie "philippines" "ph";
        Trie.searchExact "philippines" = Some("ph")
      ]}
  *)

  val searchPartial : 'v t -> string -> 'v array
  (** [searchPartial trie key] searches for a partial match of the [key] in the [trie].
 
      Example:

      {@ocaml[
        let trie = Trie.create () in
        Trie.insert trie "united states" "us";
        Trie.insert trie "united kingdom" "uk";
        Trie.searchPartial "united" = [|"us", "uk"|]
      ]}
  *)
end
