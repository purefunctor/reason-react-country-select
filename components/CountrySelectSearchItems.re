open Models;

module KeyListeners = {
  type document;

  let document: document = [%mel.raw "document"];

  type event;

  [@mel.send]
  external addEventListener: (document, string, event => unit) => unit =
    "addEventListener";

  [@mel.send]
  external removeEventListener: (document, string, event => unit) => unit =
    "removeEventListener";

  let useOnKeyDown = f => {
    React.useEffect0(() => {
      addEventListener(document, "keydown", f);
      Some(() => removeEventListener(document, "keydown", f));
    });
  };
};

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };
};

type state = {
  currentIndex: int,
  currentCountries: array(Country.t),
};

type action =
  | GoUp
  | GoDown
  | SetCountries(array(Country.t));

let reducer = (state: state, action: action) => {
  switch (action) {
  | GoUp =>
    let minimumIndex = 0;
    let currentIndex = state.currentIndex - 1;
    {...state, currentIndex: Js.Math.max_int(minimumIndex, currentIndex)};
  | GoDown =>
    let currentIndex = state.currentIndex + 1;
    let maximumIndex = Js.Array.length(state.currentCountries);
    {...state, currentIndex: Js.Math.min_int(currentIndex, maximumIndex)};
  | SetCountries(currentCountries) =>
    let currentIndex = 0;
    {currentIndex, currentCountries};
  };
};

[@react.component]
let make = (~index as _, ~search, ~onSelect, ~countryData) => {
  let CountryApi.{countryList, countryTrie, _} = countryData;

  let ({currentIndex: _, currentCountries}, dispatch) =
    React.useReducer(
      reducer,
      {currentIndex: 0, currentCountries: countryList},
    );

  // Update currentCountries only when the searchField is changed.
  let (prevSearch, setPrevSearch) = React.useState(() => search);
  if (search !== prevSearch) {
    setPrevSearch(_ => search);
    let countryList =
      Country.searchCountries(~search, ~countryList, ~countryTrie);
    dispatch(SetCountries(countryList));
  };

  currentCountries
  |> Array.mapi((index, country: Country.t) => {
       let key = string_of_int(index);
       <Item key country onSelect />;
     })
  |> React.array;
};
