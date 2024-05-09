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
      ~onChange: Country.t => unit=_ => (),
    ) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  let ({current, toggled}, dispatch) =
    React.useReducer(reducer, {current: None, toggled: false});

  // We need useEffect here to prevent `onChange` from modifying
  // state in the parent while this component is being rendered.
  React.useEffect1(
    () => {
      switch (countriesQuery) {
      | Pending
      | Failed(_) => ()
      | Finished(countryData) =>
        let country = getInitialCountry(~country, ~countryData);
        dispatch(Current(country));
        switch (country) {
        | Some(country) => onChange(country)
        | None => ()
        };
      };
      None;
    },
    [|countriesQuery|],
  );

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
    onChange(country);
  };

  <div ?className>
    <button onClick> {React.string(buttonText)} </button>
    {toggled ? <CountrySelectSearch onExit onSelect /> : React.null}
  </div>;
};
