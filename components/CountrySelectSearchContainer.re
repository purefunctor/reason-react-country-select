[@mel.module "./CountrySelectSearchContainer.module.css"]
external css: Js.t({..}) = "default";

[@react.component]
let make =
    (
      ~onChange=?,
      ~onKeyDown=?,
      ~value=?,
      ~onClearKeyDown=?,
      ~onClearClick=?,
      ~children,
    ) => {
  <div className=css##searchContainer>
    <div className=css##searchInput>
      <Icons.Search />
      <input placeholder="Search" autoFocus=true ?onChange ?onKeyDown ?value />
      <button
        title="Clear Selection"
        tabIndex=0
        className=css##searchClear
        onKeyDown=?onClearKeyDown
        onClick=?onClearClick>
        <Icons.Close />
      </button>
    </div>
    children
  </div>;
};
