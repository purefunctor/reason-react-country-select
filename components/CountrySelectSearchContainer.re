[@mel.module "./CountrySelectSearchContainer.module.css"]
external css: Js.t({..}) = "default";

[@react.component]
let make = (~onChange=?, ~onKeyDown=?, ~value=?, ~children) => {
  <div className=css##searchContainer>
    <div className=css##searchInput>
      <Icons.Search />
      <input placeholder="Search" autoFocus=true ?onChange ?onKeyDown ?value />
    </div>
    children
  </div>;
};
