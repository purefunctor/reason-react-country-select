[@mel.module "./CountrySelect.module.css"]
external css: Js.t({..}) = "default";

open Models;
open React.Event;

let getInitialCountry =
    (~country: option(string), ~countryData: CountryApi.countryData) => {
  let CountryApi.{countryList, countryValueMap, countryTrie} = countryData;
  switch (country) {
  | None => None
  | Some(country) =>
    switch (Js.Dict.get(countryValueMap, country)) {
    | Some(initialCountryIndex) => Some(countryList[initialCountryIndex])
    | None =>
      let countries =
        Country.searchCountries(~search=country, ~countryList, ~countryTrie);
      switch (countries[0]) {
      | country => Some(country)
      | exception _ => None
      };
    }
  };
};

let useClickOutside =
    (elementRef: React.ref(Js.Nullable.t(Dom.element)), setToggled) => {
  open WebFFI;

  let onMouseDown =
    React.useCallback1(
      event => {
        let child = event |> Mouse.target;
        elementRef.current
        |> Js.toOption
        |> Option.iter(element =>
             if (!Element.contains(element, ~child)) {
               setToggled(_ => false);
             }
           );
      },
      [|elementRef|],
    );

  React.useEffect1(
    () => {
      let document = document |> Document.asEventTarget;
      Mouse.addEventListener(document, "mousedown", onMouseDown);
      Some(
        () => Mouse.removeEventListener(document, "mousedown", onMouseDown),
      );
    },
    [|elementRef|],
  );
};

let useInitialCountry =
    (
      ~countriesQuery: CountryApi.countriesQuery,
      ~country,
      ~setCurrent,
      ~onChange,
    ) => {
  // We need useEffect here to prevent `onChange` from modifying
  // state in the parent while this component is being rendered.
  React.useEffect1(
    () => {
      switch (countriesQuery) {
      | Pending
      | Failed(_) => ()
      | Finished(countryData) =>
        let initialCountry = getInitialCountry(~country, ~countryData);
        setCurrent(_ => initialCountry);
        onChange(initialCountry);
      };
      None;
    },
    [|countriesQuery|],
  );
};

module Button = {
  [@react.component]
  let make = (~current: option(Country.t), ~setToggled, ~buttonRef) => {
    let onClick = _ => {
      setToggled(prevToggled => !prevToggled);
    };
    let onKeyDown = event => {
      switch (event |> Keyboard.key) {
      | "ArrowDown" =>
        event |> Keyboard.preventDefault;
        setToggled(_ => true);
      | _ => ()
      };
    };
    let buttonFlag =
      switch (current) {
      | None => React.null
      | Some(current) =>
        let alpha2 = current.value;
        <Flag className=css##selectButtonFlag alpha2 />;
      };
    let buttonText =
      switch (current) {
      | None => "Select a Country"
      | Some(current) => current.label
      };
    <button
      ref={ReactDOM.Ref.domRef(buttonRef)}
      className=css##selectButton
      onClick
      onKeyDown>
      <div className=css##selectButtonInner>
        <div className=css##selectButtonContent>
          buttonFlag
          <span> {React.string(buttonText)} </span>
        </div>
        <Icons.ArrowDown />
      </div>
    </button>;
  };
};

[@react.component]
let make =
    (
      ~className: option(string)=?,
      ~country: option(string),
      ~onChange: option(Country.t) => unit=_ => (),
    ) => {
  let (current, setCurrent) = React.useState(() => None);
  let (toggled, setToggled) = React.useState(() => false);

  let buttonRef = React.useRef(Js.Nullable.null);
  let focusButton =
    React.useCallback1(
      () => {
        buttonRef.current
        |> Js.toOption
        |> Option.iter(buttonEl => {
             WebFFI.(
               buttonEl |> Element.unsafeAsHtmlElement |> HTMLElement.focus
             )
           })
      },
      [|buttonRef|],
    );
  let containerRef = React.useRef(Js.Nullable.null);
  useClickOutside(containerRef, setToggled);

  let countriesQuery = CountryApi.useCountriesQuery();
  useInitialCountry(~countriesQuery, ~country, ~setCurrent, ~onChange);

  let onOptionClick = country => {
    setToggled(_ => false);
    setCurrent(_ => country);
    onChange(country);
  };

  let onSearchEsc = () => {
    setToggled(_ => false);
    focusButton();
  };

  let onSearchEnter = country => {
    onOptionClick(country);
    focusButton();
  };

  let className =
    switch (className) {
    | None => css##select
    | Some(className) => css##select ++ " " ++ className
    };

  <div ref={ReactDOM.Ref.domRef(containerRef)} className>
    <Button current setToggled buttonRef />
    {toggled
       ? <CountrySelectSearch onOptionClick onSearchEsc onSearchEnter />
       : React.null}
  </div>;
};
