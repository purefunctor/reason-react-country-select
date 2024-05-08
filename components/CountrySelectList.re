open Models;

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };
};

[@react.component]
let make = (~searchField: string, ~onSelect) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  switch (countriesQuery) {
  | Pending => <div> {React.string("Loading countries...")} </div>
  | Failed(_) => <div> {React.string("Could not load countries!")} </div>
  | Finished({countryList, countryTrie, _}) =>
    let filteredCountries =
      Country.searchCountries(~searchField, ~countryList, ~countryTrie);
    filteredCountries
    |> Array.mapi((index, country: Country.t) => {
         let key = string_of_int(index);
         <Item key country onSelect />;
       })
    |> React.array;
  };
};
