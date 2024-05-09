open Models;

type state = {
  current: option(Country.t),
  toggled: bool,
};

type action =
  | Current(option(Country.t))
  | Toggle;

let reducer = (state: state, action: action) => {
  switch (action) {
  | Current(current) => {current, toggled: false}
  | Toggle => {...state, toggled: !state.toggled}
  };
};

let getInitialCountry =
    (~country: option(string), ~countryData: CountryApi.countryData) => {
  let CountryApi.{countryList, countryValueMap, countryTrie} = countryData;
  switch (country) {
  | None => None
  | Some(country) =>
    switch (Js.Dict.get(countryValueMap, country)) {
    | Some(initialCountryIndex) => Some(countryList[initialCountryIndex])
    | None =>
      let countries =
        Country.searchCountries(~search=country, ~countryList, ~countryTrie);
      switch (countries[0]) {
      | country => Some(country)
      | exception _ => None
      };
    }
  };
};

[@react.component]
let make =
    (
      ~className: option(string)=?,
      ~country: option(string),
      ~onChange: string => unit=_ => (),
    ) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  let ({current, toggled}, dispatch) =
    React.useReducer(reducer, {current: None, toggled: false});

  let (prevQuery, setPrevQuery) = React.useState(() => countriesQuery);
  if (countriesQuery !== prevQuery) {
    setPrevQuery(_ => countriesQuery);
    switch (countriesQuery) {
    | Pending
    | Failed(_) => ()
    | Finished(countryData) =>
      dispatch(Current(getInitialCountry(~country, ~countryData)))
    };
  };

  let onClick = _ => dispatch(Toggle);
  let buttonText =
    switch (current) {
    | None => "Select a Country"
    | Some(current) => current.label
    };

  let onExit = _ => dispatch(Toggle);
  let onSelect = (country: Country.t) => {
    dispatch(Toggle);
    dispatch(Current(Some(country)));
    onChange(country.label);
  };

  <div ?className>
    <button onClick> {React.string(buttonText)} </button>
    {toggled ? <CountrySelectSearch onExit onSelect /> : React.null}
  </div>;
};
