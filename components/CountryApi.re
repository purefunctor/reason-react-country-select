open Models;

let countriesJsonLink = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

let getJsonFromLinkImpl: string => Js.Promise.t(Js.Json.t) = [%mel.raw
  {|
  (resource) => {
    return fetch(resource, { method: "GET" }).then(response => response.json());
  }
|}
];

let getCountriesJson = () => getJsonFromLinkImpl(countriesJsonLink);

type countryData = {
  countryList: array(Country.t),
  countryValueMap: Js.Dict.t(int),
  countryTrie: Search.Trie.t(int),
};

type countriesQuery =
  | Pending
  | Failed(ReactQuery.queryError)
  | Finished(countryData);

let useCountriesQuery = (): countriesQuery => {
  let ReactQuery.{isPending, error, data} =
    ReactQuery.useQuery({
      queryKey: [|"countriesQueryApi"|],
      queryFn: () => {
        let ( let* ) = (x, f) => x |> Js.Promise.then_(f);
        let* countriesJson = getCountriesJson();
        let countryList = Country.manyFromJson(countriesJson);
        let countryValueMap = Country.buildValueMap(countryList);
        let countryTrie = Country.buildIndexTrie(countryList);
        Js.Promise.resolve({countryList, countryValueMap, countryTrie});
      },
    });

  if (isPending) {
    Pending;
  } else {
    switch (error |> Js.Nullable.toOption) {
    | Some(queryError) => Failed(queryError)
    | None => Finished(data |> Js.Nullable.toOption |> Option.get)
    };
  };
};
