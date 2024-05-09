[@react.component]
let make = (~searchField, ~onSelect) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  switch (countriesQuery) {
  | Pending => <div> {React.string("Loading countries...")} </div>
  | Failed(_) => <div> {React.string("Could not load countries!")} </div>
  | Finished(countryData) =>
    <CountrySelectListItems searchField onSelect countryData />
  };
};
