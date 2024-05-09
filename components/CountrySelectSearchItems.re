open Models;

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };
};

[@react.component]
let make = (~index as _, ~search, ~onSelect, ~countryData) => {
  let CountryApi.{countryList, countryTrie, _} = countryData;
  let (countries, setCountries) = React.useState(() => countryList);

  // Update countries only when the search is changed.
  let (prevSearch, setPrevSearch) = React.useState(() => search);
  if (search !== prevSearch) {
    setPrevSearch(_ => search);
    setCountries(_ =>
      Country.searchCountries(~search, ~countryList, ~countryTrie)
    );
  };

  countries
  |> Array.mapi((index, country: Country.t) => {
       let key = string_of_int(index);
       <Item key country onSelect />;
     })
  |> React.array;
};
