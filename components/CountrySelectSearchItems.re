[@mel.module "./CountrySelectSearchItems.module.css"]
external css: Js.t({..}) = "default";

open Models;

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~isSelected, ~onOptionClick) => {
    let onClick = _ => onOptionClick(country);

    let itemFlag = {
      let alpha2 = country.value;
      <Flag className=css##itemFlag alpha2 />;
    };

    let className =
      if (isSelected) {
        css##item ++ " " ++ css##itemSelected;
      } else {
        css##item;
      };

    <div className onClick>
      itemFlag
      <span> {React.string(country.label)} </span>
    </div>;
  };

  let make =
    React.memoCustomCompareProps(make, (oldProps, newProps) => {
      Country.equal(oldProps##country, newProps##country)
      &&
      oldProps##isSelected ===
      newProps##isSelected
      &&
      oldProps##onOptionClick ===
      newProps##onOptionClick
    });
};

let useKeyboardBindings =
    (
      ~onUp,
      ~onDown,
      ~onEsc,
      ~onEnter,
      inputRef: React.ref(Js.Nullable.t(Dom.element)),
    ) => {
  open WebFFI;

  let onKeyDown = event => {
    switch (event |> Keyboard.key) {
    | "ArrowUp" =>
      event |> Keyboard.preventDefault;
      onUp();
    | "ArrowDown" =>
      event |> Keyboard.preventDefault;
      onDown();
    | "Escape" => onEsc()
    | "Enter" => onEnter()
    | _ => ()
    };
  };

  React.useEffect2(
    () => {
      inputRef.current
      |> Js.toOption
      |> Option.map(inputEl => {
           let inputEl = inputEl |> Element.asEventTarget;
           Keyboard.addEventListener(inputEl, "keydown", onKeyDown);
           () => Keyboard.removeEventListener(inputEl, "keydown", onKeyDown);
         })
    },
    (inputRef, onKeyDown),
  );
};

let useInputNavigation =
    (
      ~countries,
      ~search,
      ~inputRef,
      ~virtualizer,
      ~onSearchEsc,
      ~onSearchEnter,
    ) => {
  let totalCountries = Js.Array.length(countries);
  let hasCountries = totalCountries > 0;

  let (inputIndex, setInputIndex) = React.useState(() => None);

  let (prevSearch, setPrevSearch) = React.useState(() => search);
  if (search !== prevSearch) {
    setPrevSearch(_ => search);
    setInputIndex(_ => None);
  };

  let onUp = () => {
    setInputIndex(prevIndex => {
      let nextIndex =
        switch (prevIndex) {
        | None => hasCountries ? Some((totalCountries - 1, "center")) : None
        | Some(prevIndex) =>
          let isStart = prevIndex === 0;
          isStart ? None : Some((prevIndex - 1, "auto"));
        };
      switch (nextIndex) {
      | None => None
      | Some((nextIndex, align)) =>
        ReactVirtual.scrollToIndex(virtualizer, nextIndex, {align: align});
        Some(nextIndex);
      };
    });
  };
  let onDown = () => {
    setInputIndex(prevIndex => {
      let nextIndex =
        switch (prevIndex) {
        | None => hasCountries ? Some((0, "center")) : None
        | Some(prevIndex) =>
          let isFinal = prevIndex === totalCountries - 1;
          isFinal ? None : Some((prevIndex + 1, "auto"));
        };
      switch (nextIndex) {
      | None => None
      | Some((nextIndex, align)) =>
        ReactVirtual.scrollToIndex(virtualizer, nextIndex, {align: align});
        Some(nextIndex);
      };
    });
  };
  let onEsc = onSearchEsc;
  let onEnter = () => {
    switch (inputIndex) {
    | None => ()
    | Some(inputIndex) => onSearchEnter(countries[inputIndex])
    };
  };
  useKeyboardBindings(~onUp, ~onDown, ~onEsc, ~onEnter, inputRef);

  inputIndex;
};

[@react.component]
let make =
    (
      ~inputRef: React.ref(Js.Nullable.t(Dom.element)),
      ~search: string,
      ~countryData: CountryApi.countryData,
      ~onOptionClick: Country.t => unit,
      ~onSearchEsc: unit => unit,
      ~onSearchEnter: Country.t => unit,
    ) => {
  let CountryApi.{countryList, countryTrie, _} = countryData;
  let countries =
    React.useMemo3(
      () => Country.searchCountries(~search, ~countryList, ~countryTrie),
      (search, countryList, countryTrie),
    );

  let totalCountries = Js.Array.length(countries);
  let listRef = React.useRef(Js.Nullable.null);

  // estimateSize: lineHeight + paddingTop + paddingBottom
  let virtualizer =
    ReactVirtual.useVirtualizer({
      count: totalCountries,
      getScrollElement: () =>
        listRef.current |> Js.Nullable.toOption |> Option.get,
      estimateSize: () => 18 + 4 + 4,
      getItemKey: index => countries[index].label,
    });

  let inputIndex =
    useInputNavigation(
      ~countries,
      ~search,
      ~inputRef,
      ~virtualizer,
      ~onSearchEsc,
      ~onSearchEnter,
    );

  let makeItem =
    React.useCallback2(
      (itemIndex, country) => {
        let isSelected =
          switch (inputIndex) {
          | None => false
          | Some(inputIndex) => inputIndex === itemIndex
          };
        <Item country isSelected onOptionClick />;
      },
      (inputIndex, onOptionClick),
    );
  <CountrySelectSearchItemsVirtual countries virtualizer listRef makeItem />;
};
