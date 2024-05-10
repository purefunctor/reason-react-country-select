[@mel.module "./CountrySelectSearchItems.module.css"]
external css: Js.t({..}) = "default";

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
    let alpha2 = country.value;

    <div className=css##item onClick>
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
  let (index, setIndex) = React.useState(() => 0);

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
    setIndex(prevIndex => {
      let nextIndex = Js.Math.max_int(0, prevIndex - 1);
      ReactVirtual.scrollToIndex(virtualizer, nextIndex);
      nextIndex;
    });
  };
  let onDown = () => {
    setIndex(prevIndex => {
      let nextIndex = Js.Math.min_int(prevIndex + 1, length - 1);
      ReactVirtual.scrollToIndex(virtualizer, nextIndex);
      nextIndex;
    });
  };
  let onEsc = () => onExit();
  let onEnter = () => onSelect(countries[index]);
  useKeyboardBindings(~onUp, ~onDown, ~onEsc, ~onEnter, inputRef);

  let makeItem =
    React.useCallback1(country => <Item country onSelect />, [|onSelect|]);
  <CountrySelectSearchItemsVirtual countries virtualizer listRef makeItem />;
};
