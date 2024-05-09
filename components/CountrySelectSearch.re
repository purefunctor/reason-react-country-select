open React.Event;

[@react.component]
let make = (~onSelect) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  let (searchField, setSearchField) = React.useState(() => "");

  let searchFieldOnChange = event => {
    setSearchField(_ => Form.target(event)##value);
  };

  switch (countriesQuery) {
  | Pending => <div> {React.string("Loading countries...")} </div>
  | Failed(_) => <div> {React.string("Could not load countries!")} </div>
  | Finished(countryData) =>
    <div>
      <input
        placeholder="Search"
        onChange=searchFieldOnChange
        value=searchField
      />
      <CountrySelectSearchItems searchField onSelect countryData />
    </div>
  };
};
