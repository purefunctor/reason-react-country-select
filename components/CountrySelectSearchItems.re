[@mel.module "./CountrySelectSearchItems.module.css"]
external css: Js.t({..}) = "default";

open Models;

module Item = {
  let equalProps = (oldProps, newProps) => {
    let oldCountry: Country.t = oldProps##country;
    let newCountry: Country.t = newProps##country;
    let oldOnSelect = oldProps##onSelect;
    let newOnSelect = newProps##onSelect;
    let oldSelected = oldProps##isSelected;
    let newSelected = newProps##isSelected;

    oldCountry.value === newCountry.value
    && oldCountry.label === oldCountry.label
    && oldOnSelect === newOnSelect
    && oldSelected === newSelected;
  };

  [@react.component]
  let make = (~isSelected: bool, ~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    let alpha2 = country.value;

    let itemCss = ref(css##item);
    if (isSelected) {
      itemCss := itemCss^ ++ " " ++ css##itemSelected;
    };

    <div className=itemCss^ onClick>
      <Flag className=css##itemFlag alpha2 />
      <span> {React.string(country.label)} </span>
    </div>;
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
    | "ArrowUp" =>
      KeyboardFFI.preventDefault(event);
      onUp();
    | "ArrowDown" =>
      KeyboardFFI.preventDefault(event);
      onDown();
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

module Header = {
  [@react.component]
  let make = () => {
    <div className=css##listHeader />;
  };
};

module Footer = {
  [@react.component]
  let make = () => {
    <div className=css##listFooter />;
  };
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
  let (selected, setSelected) = React.useState(() => None);

  let (prevSearch, setPrevSearch) = React.useState(() => search);
  if (search !== prevSearch) {
    setPrevSearch(_ => search);
    setSelected(_ => None);
  };

  // estimateSize: lineHeight + paddingTop + paddingBottom
  let virtualizer =
    ReactVirtual.useVirtualizer({
      count: length,
      getScrollElement: () =>
        listRef.current |> Js.Nullable.toOption |> Option.get,
      estimateSize: () => 18 + 4 + 4,
      getItemKey: index => countries[index].label,
    });

  let onUp = () => {
    setSelected(prevSelected => {
      switch (prevSelected) {
      | None => None
      | Some(prevSelected) =>
        let nextSelected = Js.Math.max_int(0, prevSelected - 1);
        ReactVirtual.scrollToIndex(virtualizer, nextSelected);
        Some(nextSelected);
      }
    });
  };
  let onDown = () => {
    setSelected(prevSelected => {
      switch (prevSelected) {
      | None => length === 0 ? None : Some(0)
      | Some(prevSelected) =>
        let nextSelected = Js.Math.min_int(prevSelected + 1, length - 1);
        ReactVirtual.scrollToIndex(virtualizer, nextSelected);
        Some(nextSelected);
      }
    });
  };
  let onEsc = () => onExit();
  let onEnter = () => {
    switch (selected) {
    | None => ()
    | Some(index) => onSelect(countries[index])
    };
  };
  useKeyboardBindings(~onUp, ~onDown, ~onEsc, ~onEnter, inputRef);

  let makeItem =
    React.useCallback2(
      (itemIndex, country) => {
        let isSelected =
          switch (selected) {
          | None => false
          | Some(index) => index === itemIndex
          };
        <Item isSelected country onSelect />;
      },
      (selected, onSelect),
    );
  <CountrySelectSearchItemsVirtual countries virtualizer listRef makeItem />;
};
