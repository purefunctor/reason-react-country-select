type virtualizer;

type virtualItem = {
  key: string,
  index: int,
  start: float,
  [@mel.as "end"]
  end_: float,
  size: float,
};

type options = {
  count: int,
  getScrollElement: unit => Dom.element,
  estimateSize: unit => int,
  getItemKey: int => string,
};

[@mel.module "@tanstack/react-virtual"]
external useVirtualizer: options => virtualizer = "useVirtualizer";

[@mel.send] external getTotalSize: virtualizer => float = "getTotalSize";
[@mel.send]
external getVirtualItems: virtualizer => array(virtualItem) =
  "getVirtualItems";

type scrollOptions = {align: string};

[@mel.send]
external scrollToIndex: (virtualizer, int, scrollOptions) => unit =
  "scrollToIndex";

[@mel.get]
external getMeasureElement: virtualizer => ReactDOM.Ref.callbackDomRef =
  "measureElement";
