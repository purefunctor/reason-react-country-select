open Models;

type state = {
  buttonText: string,
  isToggled: bool,
};

type action =
  | SelectCountry(Country.t)
  | ToggleDropdown;

let reducer = (state: state, action: action) => {
  switch (action) {
  | SelectCountry(country) => {buttonText: country.label, isToggled: false}
  | ToggleDropdown => {...state, isToggled: !state.isToggled}
  };
};

let defaultState = {buttonText: "Country", isToggled: false};

let getInitialCountry =
    (~country: option(string), ~countriesQuery: CountryApi.countriesQuery) => {
  switch (country, countriesQuery: CountryApi.countriesQuery) {
  | (
      Some(initialCountry),
      Finished({countryList, countryValueMap, countryTrie}),
    ) =>
    switch (Js.Dict.get(countryValueMap, initialCountry)) {
    | Some(initialCountryIndex) => Some(countryList[initialCountryIndex])
    | None =>
      let countries =
        Country.searchCountries(
          ~searchField=initialCountry,
          ~countryList,
          ~countryTrie,
        );
      switch (countries[0]) {
      | country => Some(country)
      | exception _ => None
      };
    }
  | _ => None
  };
};

[@react.component]
let make =
    (
      ~className: option(string)=?,
      ~country: option(string),
      ~onChange: option(string => unit)=?,
    ) => {
  let countriesQuery = CountryApi.useCountriesQuery();

  let ({buttonText, isToggled}, dispatch) =
    React.useReducer(reducer, defaultState);

  React.useEffect1(
    () => {
      let country = getInitialCountry(~country, ~countriesQuery);
      switch (country) {
      | Some(country) => dispatch(SelectCountry(country))
      | None => ()
      };
      None;
    },
    [|countriesQuery|],
  );

  let onClick = _ => {
    dispatch(ToggleDropdown);
  };

  let onChange = Option.value(onChange, ~default=_ => ());

  let onSelect = (country: Country.t) => {
    dispatch(SelectCountry(country));
    onChange(country.label);
  };

  <div ?className>
    <button onClick> {React.string(buttonText)} </button>
    {isToggled ? <CountrySelectSearch onSelect /> : React.null}
  </div>;
};
