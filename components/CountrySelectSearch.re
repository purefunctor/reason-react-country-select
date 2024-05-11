[@mel.module "./CountrySelectSearch.module.css"]
external css: Js.t({..}) = "default";

module SearchContainer = CountrySelectSearchContainer;
module SearchItems = CountrySelectSearchItems;

[@react.component]
let make = (~onOptionClick, ~onSearchEsc, ~onSearchEnter) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  switch (countriesQuery) {
  | Pending =>
    <SearchContainer>
      <div className=css##message>
        {React.string("Loading countries...")}
      </div>
    </SearchContainer>
  | Failed(_) =>
    <SearchContainer>
      <div className=css##message>
        {React.string("Could not load countries!")}
      </div>
    </SearchContainer>
  | Finished(countryData) =>
    <SearchItems countryData onOptionClick onSearchEsc onSearchEnter />
  };
};
