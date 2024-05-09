open Models;

module Item = {
  let equalProps = (oldProps, newProps) => {
    let oldCountry: Country.t = oldProps##country;
    let newCountry: Country.t = newProps##country;
    let oldOnSelect = oldProps##onSelect;
    let newOnSelect = oldProps##onSelect;

    oldCountry.value === newCountry.value
    && oldCountry.label === oldCountry.label
    && oldOnSelect === newOnSelect;
  };

  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };

  let make = React.memoCustomCompareProps(make, equalProps);
};

type state = {index: int};

type action =
  | GoUp
  | GoDown;

let reducer = (~length: int, state: state, action: action) =>
  switch (action) {
  | GoUp =>
    let minimum = 0;
    {index: Js.Math.max_int(minimum, state.index - 1)};
  | GoDown =>
    let maximum = length - 1;
    {index: Js.Math.min_int(state.index + 1, maximum)};
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
  let countries =
    React.useMemo3(
      () => Country.searchCountries(~search, ~countryList, ~countryTrie),
      (search, countryList, countryTrie),
    );

  let length = Js.Array.length(countries);
  let ({index}, dispatch) = {
    React.useReducer(reducer(~length), {index: 0});
  };

  let onUp = () => dispatch(GoUp);
  let onDown = () => dispatch(GoDown);
  let onEsc = () => onExit();
  let onEnter = () => onSelect(countries[index]);
  useKeyboardBindings(~onUp, ~onDown, ~onEsc, ~onEnter, inputRef);

  <div>
    {countries
     |> Array.mapi((index, country: Country.t) => {
          let key = string_of_int(index);
          <Item key country onSelect />;
        })
     |> React.array}
  </div>;
};
