open Models;
open Search;

let countriesJsonLink = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

let getJsonFromLinkImpl: string => Js.Promise.t(Js.Json.t) = [%mel.raw
  {|
  (resource) => {
    return fetch(resource, { method: "GET" }).then(response => response.json());
  }
|}
];

let getCountriesJson = () => getJsonFromLinkImpl(countriesJsonLink);

let searchCountries =
    (
      ~searchField: string,
      ~countriesList: array(Country.t),
      ~countriesTrie: Trie.t(int),
    )
    : array(Country.t) => {
  let searchField = searchField |> Js.String.toLowerCase;
  let countryIndices = Trie.searchPartial(countriesTrie, searchField);
  let countries = [||];
  countryIndices
  |> Array.iter(countryIndex => {
       let value = countriesList[countryIndex];
       ignore(Js.Array.push(~value, countries));
     });
  countries;
};

type countriesKind =
  | Pending
  | Failed(ReactQuery.queryError)
  | Finished(array(Country.t));

let useCountries = (~searchField: string): countriesKind => {
  let ReactQuery.{isPending, error, data: countriesData} =
    ReactQuery.useQuery({
      queryKey: [|"countriesQuery"|],
      queryFn: () => {
        let ( let* ) = (x, f) => x |> Js.Promise.then_(f);
        let* countriesJson = getCountriesJson();
        let countriesList = Country.manyFromJson(countriesJson);
        let countriesValueMap = Country.buildValueMap(countriesList);
        let countriesTrie = Country.buildIndexTrie(countriesList);
        Js.Promise.resolve((countriesList, countriesValueMap, countriesTrie));
      },
    });

  if (isPending) {
    Pending;
  } else {
    switch (error |> Js.Nullable.toOption) {
    | Some(queryError) => Failed(queryError)
    | None =>
      switch (countriesData |> Js.Nullable.toOption) {
      | None => Finished([||])
      | Some((countriesList, _, countriesTrie)) =>
        Finished(
          searchCountries(~searchField, ~countriesList, ~countriesTrie),
        )
      }
    };
  };
};
