module Virtuoso = {
  type components = {
    [@mel.as "Header"]
    header: Js.t({.}) => React.element,
    [@mel.as "Footer"]
    footer: Js.t({.}) => React.element,
  };

  [@mel.module "react-virtuoso"] [@react.component]
  external make:
    (
      ~key: string=?,
      ~ref: ReactDOM.domRef=?,
      ~style: ReactDOM.style=?,
      ~components: components=?,
      ~totalCount: int,
      ~itemContent: int => React.element,
      ~totalListHeightChanged: float => unit=?,
    ) =>
    React.element =
    "Virtuoso";
};
