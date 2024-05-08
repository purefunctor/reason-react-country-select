open Models;
open Search;

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };
};

[@react.component]
let make = (~searchField: string, ~onSelect) => {
  // TODO: extract behaviour to a custom hook, fetch countries from the API
  let countries =
    React.useMemo1(
      () => {
        let searchField = searchField |> Js.String.toLowerCase;
        let countryIndices =
          Trie.searchPartial(Country.exampleCountriesTrie, searchField);
        let countries = [||];
        countryIndices
        |> Array.iter(countryIndex => {
             let value = Country.exampleCountries[countryIndex];
             ignore(Js.Array.push(~value, countries));
           });
        countries;
      },
      [|searchField|],
    );

  <div>
    {countries
     |> Array.mapi((index, country: Country.t) => {
          let key = string_of_int(index);
          <Item key country onSelect />;
        })
     |> React.array}
  </div>;
};
