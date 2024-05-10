[@mel.module "./CountrySelect.module.css"]
external css: Js.t({..}) = "default";

open Models;

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
  open EventFFI;

  let onMouseDown =
    React.useCallback1(
      (event: Mouse.event) => {
        let target = event.target;
        elementRef.current
        |> Js.toOption
        |> Option.iter(element =>
             if (!elementContainsTarget(~element, ~target)) {
               setToggled(_ => false);
             }
           );
      },
      [|elementRef|],
    );

  React.useEffect1(
    () => {
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
        switch (initialCountry) {
        | Some(initialCountry) => onChange(initialCountry)
        | None => ()
        };
      };
      None;
    },
    [|countriesQuery|],
  );
};

module Button = {
  [@react.component]
  let make = (~current: option(Country.t), ~onClick) => {
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
    <button className=css##selectButton onClick>
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
      ~onChange: Country.t => unit=_ => (),
    ) => {
  let (current, setCurrent) = React.useState(() => None);
  let (toggled, setToggled) = React.useState(() => false);

  let containerRef = React.useRef(Js.Nullable.null);
  useClickOutside(containerRef, setToggled);

  let countriesQuery = CountryApi.useCountriesQuery();
  useInitialCountry(~countriesQuery, ~country, ~setCurrent, ~onChange);

  let onClick = _ => setToggled(prevToggled => !prevToggled);
  let onExit = _ => setToggled(_ => false);
  let onSelect = (country: Country.t) => {
    setToggled(_ => false);
    setCurrent(_ => Some(country));
    onChange(country);
  };

  let className =
    switch (className) {
    | None => css##select
    | Some(className) => css##select ++ " " ++ className
    };

  <div ref={ReactDOM.Ref.domRef(containerRef)} className>
    <Button current onClick />
    {toggled ? <CountrySelectSearch onExit onSelect /> : React.null}
  </div>;
};
