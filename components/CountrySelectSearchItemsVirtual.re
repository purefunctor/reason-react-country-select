open Models;

// estimateHeight: 26px
// maxElements: 14
// padding: 4px
// estimateHeight * maxElements - padding = 360px
let containerStyle =
  ReactDOM.Style.make(
    ~maxHeight="360px",
    ~overflow="auto",
    ~padding="4px 0 4px 0",
    (),
  );

let innerStyle = (~innerHeight) => {
  ReactDOM.Style.make(
    ~height=string_of_int(innerHeight) ++ "px",
    ~width="100%",
    ~position="relative",
    (),
  );
};

let itemStyle = (~start) => {
  ReactDOM.Style.make(
    ~position="absolute",
    ~top="0",
    ~left="0",
    ~width="100%",
    ~transform=
      "translateY(" ++ (start |> int_of_float |> string_of_int) ++ "px)",
    (),
  );
};

[@react.component]
let make =
    (
      ~countries: array(Country.t),
      ~virtualizer: ReactVirtual.virtualizer,
      ~listRef: ReactDOM.Ref.currentDomRef,
      ~makeItem: (int, Country.t) => React.element,
    ) => {
  let innerHeight = virtualizer |> ReactVirtual.getTotalSize |> int_of_float;

  <div tabIndex=(-1) ref={ReactDOM.Ref.domRef(listRef)} style=containerStyle>
    <div style={innerStyle(~innerHeight)}>
      {ReactVirtual.getVirtualItems(virtualizer)
       |> Array.map((virtualItem: ReactVirtual.virtualItem) => {
            let ReactVirtual.{index, key, start, _} = virtualItem;
            let measureElement =
              virtualizer
              |> ReactVirtual.getMeasureElement
              |> ReactDOM.Ref.callbackDomRef;
            React.cloneElement(
              <div />,
              {
                "data-index": index,
                "ref": measureElement,
                "key": key,
                "style": itemStyle(~start),
                "children": makeItem(index, countries[index]),
              },
            );
          })
       |> React.array}
    </div>
  </div>;
};
