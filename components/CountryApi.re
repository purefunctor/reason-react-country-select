open Models;

let countriesJsonLink = "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json";

let getJsonFromLinkImpl: string => Js.Promise.t(Js.Json.t) =
  url => {
    Fetch.fetchWithInit(url, Fetch.RequestInit.make(~method_=Fetch.Get, ()))
    |> Js.Promise.then_(Fetch.Response.json);
  };

let getCountriesJson = () => {
  let ( let* ) = (x, f) => x |> Js.Promise.then_(f);
  let* json = getJsonFromLinkImpl(countriesJsonLink);

  // FAKE DATA
  let lcg = LCG.makeLcgRange(50_000, 250_000);
  let raw: array(Js.Dict.t(Js.Json.t)) =
    json |> Json.Decode.(array(dict(id)));
  raw
  |> Array.iter(country => {
       let count = Json.Encode.(int(lcg()));
       Js.Dict.set(country, "count", count);
     });
  let json = raw |> Json.Encode.(array(Json.Encode.jsonDict));
  // FAKE DATA

  Js.Promise.resolve(json);
};

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
        countryList
        |> Array.sort((x: Country.t, y: Country.t) =>
             Int.compare(y.count, x.count)
           );
        let countryValueMap = Country.buildValueMap(countryList);
        let countryTrie = Country.buildIndexTrie(countryList);
        Js.Promise.resolve({countryList, countryValueMap, countryTrie});
      },
    });
  React.useMemo3(
    () =>
      if (isPending) {
        Pending;
      } else {
        switch (error |> Js.Nullable.toOption) {
        | Some(queryError) => Failed(queryError)
        | None => Finished(data |> Js.Nullable.toOption |> Option.get)
        };
      },
    (isPending, error, data),
  );
};
