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
  let listRef = React.useRef(Js.Nullable.null);
  let (index, setIndex) = React.useState(() => 0);

  let scrollToIndex =
    React.useCallback1(
      index => {
        switch (listRef.current |> Js.Nullable.toOption) {
        | None => ()
        | Some(listRef) =>
          Obj.magic(listRef)##scrollIntoView({
            "index": index,
            "behaviour": "auto",
          })
        }
      },
      [|listRef|],
    );

  let onUp = () => {
    setIndex(prevIndex => {
      let nextIndex = Js.Math.max_int(0, prevIndex - 1);
      scrollToIndex(nextIndex);
      nextIndex;
    });
  };
  let onDown = () => {
    setIndex(prevIndex => {
      let nextIndex = Js.Math.min_int(prevIndex + 1, length - 1);
      scrollToIndex(nextIndex);
      nextIndex;
    });
  };
  let onEsc = () => onExit();
  let onEnter = () => onSelect(countries[index]);
  useKeyboardBindings(~onUp, ~onDown, ~onEsc, ~onEnter, inputRef);

  <ReactVirtuoso.Virtuoso
    ref={ReactDOM.Ref.domRef(listRef)}
    style={ReactDOM.Style.make(~height="400px", ())}
    totalCount={Js.Array.length(countries)}
    itemContent={index => {
      let country = countries[index];
      let key = country.value;
      <Item key country onSelect />;
    }}
  />;
};
