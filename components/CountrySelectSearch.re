[@mel.module "./CountrySelectSearch.module.css"]
external css: Js.t({..}) = "default";

open React.Event;

[@react.component]
let make = (~onExit, ~onSelect) => {
  let countriesQuery = CountryApi.useCountriesQuery();
  let (search, setSearch) = React.useState(() => "");

  let inputRef = React.useRef(Js.Nullable.null);
  let onChange = event => {
    setSearch(_ => Form.target(event)##value);
  };

  <div className=css##searchContainer>
    <div className=css##searchInput>
      <Icons.Search />
      <input
        ref={ReactDOM.Ref.domRef(inputRef)}
        placeholder="Search"
        onChange
        value=search
      />
    </div>
    {switch (countriesQuery) {
     | Pending => <div> {React.string("Loading countries...")} </div>
     | Failed(_) => <div> {React.string("Could not load countries!")} </div>
     | Finished(countryData) =>
       <CountrySelectSearchItems inputRef search countryData onExit onSelect />
     }}
  </div>;
};
