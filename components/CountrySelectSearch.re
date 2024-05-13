[@mel.module "./CountrySelectSearch.module.css"]
external css: Js.t({..}) = "default";

open React.Event;

module SearchContainer = CountrySelectSearchContainer;
module SearchItems = CountrySelectSearchItems;

[@react.component]
let make = (~onOptionClick, ~onSearchEsc, ~onSearchEnter) => {
  let countriesQuery = CountryApi.useCountriesQuery();

  let onEmptyKeyDown = event => {
    switch (event |> Keyboard.key) {
    | "Escape"
    | "Enter" =>
      event |> Keyboard.preventDefault;
      onSearchEsc();
    | _ => ()
    };
  };

  switch (countriesQuery) {
  | Pending =>
    <SearchContainer onKeyDown=onEmptyKeyDown onClearKeyDown=onEmptyKeyDown>
      <div className=css##message>
        {React.string("Loading countries...")}
      </div>
    </SearchContainer>
  | Failed(_) =>
    <SearchContainer onKeyDown=onEmptyKeyDown onClearKeyDown=onEmptyKeyDown>
      <div className=css##message>
        {React.string("Could not load countries!")}
      </div>
    </SearchContainer>
  | Finished(countryData) =>
    <SearchItems countryData onOptionClick onSearchEsc onSearchEnter />
  };
};
