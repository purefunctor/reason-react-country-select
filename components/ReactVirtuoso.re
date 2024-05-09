module Virtuoso = {
  [@mel.module "react-virtuoso"] [@react.component]
  external make:
    (
      ~key: string=?,
      ~ref: ReactDOM.domRef=?,
      ~style: ReactDOM.style=?,
      ~totalCount: int,
      ~itemContent: int => React.element
    ) =>
    React.element =
    "Virtuoso";
};
