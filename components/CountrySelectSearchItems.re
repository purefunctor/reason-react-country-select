[@mel.module "./CountrySelectSearchItems.module.css"]
external css: Js.t({..}) = "default";

open Models;
open React.Event;

module SearchContainer = CountrySelectSearchContainer;
module SearchItemsVirtual = CountrySelectSearchItemsVirtual;

let nf =
  WebFFI.Intl.NumberFormat.make(
    "en",
    {notation: "compact", minimumFractionDigits: 1, maximumFractionDigits: 1},
  );

module SearchItem = {
  [@react.component]
  let make = (~country: Country.t, ~isSelected, ~onOptionClick) => {
    let onClick = _ => onOptionClick(Some(country));

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

    let label = country.label;
    let count = WebFFI.Intl.NumberFormat.format(nf, `int(country.count));

    <div className onClick>
      itemFlag
      <span className=css##itemLabel> {React.string(label)} </span>
      <span className=css##itemCount> {React.string(count)} </span>
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

let useKeyboardNavigation =
    (~countries, ~search, ~virtualizer, ~onSearchEsc, ~onSearchEnter) => {
  let totalCountries = Js.Array.length(countries);
  let hasCountries = totalCountries > 0;

  let (searchIndex, setSearchIndex) = React.useState(() => None);

  let (prevSearch, setPrevSearch) = React.useState(() => search);
  if (search !== prevSearch) {
    let emptySearch = search === "";
    setPrevSearch(_ => search);
    setSearchIndex(_ =>
      if (emptySearch) {
        None;
      } else if (hasCountries) {
        Some(0);
      } else {
        None;
      }
    );
  };

  let onUp = () => {
    setSearchIndex(prevIndex => {
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
    setSearchIndex(prevIndex => {
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
    switch (searchIndex) {
    | None => ()
    | Some(searchIndex) => onSearchEnter(Some(countries[searchIndex]))
    };
  };
  let onSearchKeyDown = event => {
    let (callback, hasCallback) =
      switch (event |> Keyboard.key) {
      | "ArrowUp" => (onUp, true)
      | "ArrowDown" => (onDown, true)
      | "Escape" => (onEsc, true)
      | "Enter" => (onEnter, true)
      | _ => ((() => ()), false)
      };
    if (hasCallback) {
      event |> Keyboard.preventDefault;
    };
    callback();
  };

  let onClearKeyDown = event => {
    switch (event |> Keyboard.key) {
    | "Escape" =>
      event |> Keyboard.preventDefault;
      onSearchEsc();
    | "Enter" =>
      event |> Keyboard.preventDefault;
      onSearchEnter(None);
    | _ => ()
    };
  };

  (searchIndex, onSearchKeyDown, onClearKeyDown);
};

[@react.component]
let make = (~countryData, ~onOptionClick, ~onSearchEsc, ~onSearchEnter) => {
  let (search, setSearch) = React.useState(() => "");
  let onSearchChange = event => {
    setSearch(_ => Form.target(event)##value);
  };

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

  let (searchIndex, onSearchKeyDown, onClearKeyDown) =
    useKeyboardNavigation(
      ~countries,
      ~search,
      ~virtualizer,
      ~onSearchEsc,
      ~onSearchEnter,
    );

  let onClearClick = _ => onOptionClick(None);

  let makeItem =
    React.useCallback2(
      (itemIndex, country) => {
        let isSelected =
          switch (searchIndex) {
          | None => false
          | Some(inputIndex) => inputIndex === itemIndex
          };
        <SearchItem country isSelected onOptionClick />;
      },
      (searchIndex, onOptionClick),
    );

  <SearchContainer
    onChange=onSearchChange
    onKeyDown=onSearchKeyDown
    onClearKeyDown
    onClearClick
    value=search>
    <SearchItemsVirtual countries virtualizer listRef makeItem />
  </SearchContainer>;
};
