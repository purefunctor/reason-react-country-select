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
  let countriesKind = CountrySelectListHooks.useCountries(~searchField);
  switch (countriesKind) {
  | Pending => <div> {React.string("Loading countries...")} </div>
  | Failed(_) => <div> {React.string("Could not load countries!")} </div>
  | Finished(countries) =>
    countries
    |> Array.mapi((index, country: Country.t) => {
         let key = string_of_int(index);
         <Item key country onSelect />;
       })
    |> React.array
  };
};
