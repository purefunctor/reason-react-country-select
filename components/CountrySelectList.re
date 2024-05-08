open Models;

module Item = {
  [@react.component]
  let make = (~country: Country.t, ~onSelect) => {
    let onClick = _ => onSelect(country);
    <div onClick> {React.string(country.label)} </div>;
  };
};

[@react.component]
let make = (~searchField: string, ~onSelect) => {
  let startsWithSearchField = str => {
    let prefix = searchField |> Js.String.toLowerCase;
    Js.String.startsWith(~prefix, str);
  };

  // TODO: Functional-style filtering, while very simple, also does a lot of
  // redundant work. Each change iterates the entirety of `exampleCountries`
  // which leads to redundant checks. A dedicated search data structure would
  // be much more appropriate for this.
  let countries =
    React.useMemo1(
      () => {
        let f = (country: Country.t) => {
          let label = country.label |> Js.String.toLowerCase;
          // we assume that `value = toLowerCase(value)`
          // but this redundancy doesn't hurt that much
          let value = country.value |> Js.String.toLowerCase;
          startsWithSearchField(label) || startsWithSearchField(value);
        };
        // TODO: obtain these countries from the API.
        Country.exampleCountries |> Js.Array.filter(~f);
      },
      [|searchField|],
    );

  <div>
    {countries
     |> Array.mapi((index, country: Country.t) => {
          let key = string_of_int(index);
          <Item key country onSelect />;
        })
     |> React.array}
  </div>;
};
