type t = {
  value: string,
  label: string,
};

let equal = (l: t, r: t) => {
  l.value === r.value && l.label === r.label;
};

let fromJson = (json: Js.Json.t): t => {
  let country = Obj.magic(json);
  let value = country##value;
  let label = country##label;
  {value, label};
};

let manyFromJson = (json: Js.Json.t): array(t) => {
  let countries = Obj.magic(json);
  Array.map(fromJson, countries);
};

let normalize = {
  let regexp = [%mel.re "/\\p{Diacritic}/gu"];
  let replacement = "";
  value => {
    value
    |> Js.String.toLowerCase
    |> Js.String.normalize(~form=`NFD)
    |> Js.String.replaceByRe(~regexp, ~replacement);
  };
};

let buildValueMap = (countries: array(t)): Js.Dict.t(int) => {
  let map = Js.Dict.empty();
  countries
  |> Array.iteri((index, country) => {
       Js.Dict.set(map, country.value, index)
     });
  map;
};

let buildIndexTrie = (countries: array(t)): Search.Trie.t(int) => {
  let trie = Search.Trie.create();
  countries
  |> Array.iteri((index, country) => {
       let label = country.label |> normalize;
       Search.Trie.insert(trie, label, index);
     });
  trie;
};

let searchCountries =
    (
      ~search: string,
      ~countryList: array(t),
      ~countryTrie: Search.Trie.t(int),
    )
    : array(t) =>
  if (search === "") {
    countryList;
  } else {
    let search = search |> normalize;
    let countryIndices = Search.Trie.searchPartial(countryTrie, search);
    let countries = [||];
    countryIndices
    |> Array.iter(countryIndex => {
         let value = countryList[countryIndex];
         ignore(Js.Array.push(~value, countries));
       });
    countries;
  };
