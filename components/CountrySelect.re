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

let defaultState = {buttonText: "Select a Country", isToggled: false};

[@react.component]
let make =
    (
      ~className: option(string)=?,
      ~country as _: option(string),
      ~onChange: option(string => unit)=?,
    ) => {
  let ({buttonText, isToggled}, dispatch) =
    React.useReducer(reducer, defaultState);

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
