[@mel.module "./CountrySelectSearchContainer.module.css"]
external css: Js.t({..}) = "default";

[@react.component]
let make = (~onChange=?, ~onKeyDown=?, ~value=?, ~children) => {
  <div className=css##searchContainer>
    <div className=css##searchInput>
      <Icons.Search />
      <input placeholder="Search" ?onChange ?onKeyDown ?value />
    </div>
    children
  </div>;
};
