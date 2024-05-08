open React.Event;

[@react.component]
let make = (~onSelect) => {
  let (searchField, setSearchField) = React.useState(() => "");

  let searchFieldOnChange = event => {
    setSearchField(_ => Form.target(event)##value);
  };

  <div>
    <input
      placeholder="Search"
      onChange=searchFieldOnChange
      value=searchField
    />
    <CountrySelectList searchField onSelect />
  </div>;
};
