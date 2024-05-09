open Models;

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };
};

type state = {
  index: int,
  countries: array(Country.t),
};

type action =
  | GoUp
  | GoDown
  | SetCountries(array(Country.t));

let reducer = (state: state, action: action) =>
  switch (action) {
  | GoUp =>
    let minimum = 0;
    {...state, index: Js.Math.max_int(minimum, state.index - 1)};
  | GoDown =>
    let maximum = Js.Array.length(state.countries) - 1;
    {...state, index: Js.Math.min_int(state.index + 1, maximum)};
  | SetCountries(countries) =>
    let index = 0;
    {index, countries};
  };

let useKeyboardBindings =
    (
      ~onUp,
      ~onDown,
      ~onEsc,
      ~onEnter,
      inputRef: React.ref(Js.Nullable.t(Dom.element)),
    ) => {
  let onKeyDown = (event: KeyboardFFI.event) => {
    switch (event.key) {
    | "ArrowUp" => onUp()
    | "ArrowDown" => onDown()
    | "Escape" => onEsc()
    | "Enter" => onEnter()
    | _ => ()
    };
  };
  React.useEffect2(
    () => {
      switch (inputRef.current |> Js.toOption) {
      | None => None
      | Some(element) =>
        open KeyboardFFI;
        addEventListener(element, "keydown", onKeyDown);
        Some(() => removeEventListener(element, "keydown", onKeyDown));
      }
    },
    (inputRef, onKeyDown),
  );
};

[@react.component]
let make =
    (
      ~inputRef: React.ref(Js.Nullable.t(Dom.element)),
      ~search: string,
      ~countryData: CountryApi.countryData,
      ~onExit: unit => unit,
      ~onSelect: Country.t => unit,
    ) => {
  let CountryApi.{countryList, countryTrie, _} = countryData;
  let ({index, countries}, dispatch) = {
    React.useReducer(reducer, {index: 0, countries: countryList});
  };

  let onUp = () => dispatch(GoUp);
  let onDown = () => dispatch(GoDown);
  let onEsc = () => onExit();
  let onEnter = () => onSelect(countries[index]);
  useKeyboardBindings(~onUp, ~onDown, ~onEsc, ~onEnter, inputRef);

  // Update countries only when the search is changed.
  let (prevSearch, setPrevSearch) = React.useState(() => search);
  if (search !== prevSearch) {
    setPrevSearch(_ => search);
    let countries =
      Country.searchCountries(~search, ~countryList, ~countryTrie);
    dispatch(SetCountries(countries));
  };

  countries
  |> Array.mapi((index, country: Country.t) => {
       let key = string_of_int(index);
       <Item key country onSelect />;
     })
  |> React.array;
};
